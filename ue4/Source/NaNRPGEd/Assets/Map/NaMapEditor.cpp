// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPGEd.h"
#include "NaMapEditor.h"

#include "UnrealEd.h"
#include "TabManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"

#include "SDockTab.h"

#include "Viewport/SNaMapEditorViewport.h"

#include "Actions/NaMapEditorActions.h"

#define LOCTEXT_NAMESPACE "NaNRPGEd"

//====================================================================================================

const FName FNaMapEditor::s_ViewportTabId( TEXT("NaMapEditor_Viewport") );
const FName FNaMapEditor::s_PropertiesTabId( TEXT("NaMapEditor_Properties") );
const FName FNaMapEditor::s_DetailsTabId( TEXT("NaMapEditor_Details") );
const FName FNaMapEditor::s_BlockListTabId( TEXT("NaMapEditor_BlockList") );

//====================================================================================================

//! 
FNaMapEditor::FNaMapEditor()
: m_CurrentObject( nullptr )
{
}

//! 
void FNaMapEditor::AddReferencedObjects( FReferenceCollector& Collector )
{
	Collector.AddReferencedObject( m_CurrentObject );
	Collector.AddReferencedObject( m_DetailsObject );

	Collector.AddReferencedObject( m_PrevWorld );
}

//! 
FName FNaMapEditor::GetToolkitFName() const
{
	return FName("NaMapEditor");
}

//! 
FText FNaMapEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "NaMapEditor");
}

//! 
FString FNaMapEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "NaMap").ToString();
}

//! 
FLinearColor FNaMapEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor( 0.3f, 0.2f, 0.5f, 0.5f );
}

//! 
void FNaMapEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_StaticMeshEditor", "Static Mesh Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner( s_ViewportTabId, FOnSpawnTab::CreateSP(this, &FNaMapEditor::SpawnTab_Viewport) )
		.SetDisplayName( LOCTEXT("ViewportTab", "Viewport") )
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));

	InTabManager->RegisterTabSpawner( s_PropertiesTabId, FOnSpawnTab::CreateSP(this, &FNaMapEditor::SpawnTab_Properties) )
		.SetDisplayName( LOCTEXT("PropertiesTab", "Details") )
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner( s_DetailsTabId, FOnSpawnTab::CreateSP(this, &FNaMapEditor::SpawnTab_SimpleControl) )
		.SetDisplayName( LOCTEXT("EditPanelTab", "EditPanel") )
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner( s_BlockListTabId, FOnSpawnTab::CreateSP(this, &FNaMapEditor::SpawnTab_BlockList) )
		.SetDisplayName( LOCTEXT("BlockListTab", "BlockList") )
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

//! 
void FNaMapEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner( s_ViewportTabId );
	InTabManager->UnregisterTabSpawner( s_PropertiesTabId );
	InTabManager->UnregisterTabSpawner( s_DetailsTabId );
	InTabManager->UnregisterTabSpawner( s_BlockListTabId );
}

//! エディタ初期化
void FNaMapEditor::InitEditor( const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UNaMapAsset* ObjectToEdit )
{
	m_CurrentObject	= ObjectToEdit;

	m_DetailsObject	= NewObject<UNaMapEditorDetail>();
	if ( m_DetailsObject ){
		m_DetailsObject->MapSize	= m_CurrentObject->m_MapSize;
	}

	m_PrevWorld	= NewObject<UNaEditorWorld>();
	if ( m_PrevWorld ){
		m_PrevWorld->SetupFromMap( m_CurrentObject );
		m_PrevWorld->SetCurrentPosition( FIntVector( 0, 0, 0 ) );
	}

	//! 
	FNaMapEditorCommands::Register();

	{//! メインビューポート
		m_Viewport = SNew( SNaMapEditorViewport );
//			.StaticMeshEditor(SharedThis(this))
//			.ObjectToEdit(ObjectToEdit);

		m_Viewport->SetEditorWorld( m_PrevWorld );
		UpdateFloorSize();

		m_WorldActor	= m_Viewport->GetWorldActor();
		m_WorldActor->SetNaWorld( m_PrevWorld );
		m_WorldActor->UpdateInvalidateChunk();
	}

	{//! アセット詳細パネル
		FPropertyEditorModule&	PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
		FDetailsViewArgs		DetailsViewArgs;
//		FOnGetDetailCustomizationInstance	LayoutCustomStaticMeshProperties = FOnGetDetailCustomizationInstance::CreateSP( this, &FStaticMeshEditor::MakeStaticMeshDetails );

		DetailsViewArgs.bAllowSearch			= true;
		DetailsViewArgs.bLockable				= false;
		DetailsViewArgs.bUpdatesFromSelection	= false;
		DetailsViewArgs.NameAreaSettings		= FDetailsViewArgs::HideNameArea;
		DetailsViewArgs.NotifyHook				= this;
		m_AssetDetailsView = PropertyEditorModule.CreateDetailView( DetailsViewArgs );

//		m_AssetDetailsView->RegisterInstancedCustomPropertyLayout( UStaticMesh::StaticClass(), LayoutCustomStaticMeshProperties );
		m_AssetDetailsView->OnFinishedChangingProperties().AddSP( this, &FNaMapEditor::OnFinishedChangingProperties );

		m_AssetDetailsView->SetObject( m_CurrentObject );
	}

	{//! 簡易編集パネル
		FPropertyEditorModule&	PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
		FDetailsViewArgs		DetailsViewArgs;
//		FOnGetDetailCustomizationInstance	LayoutCustomStaticMeshProperties = FOnGetDetailCustomizationInstance::CreateSP( this, &FStaticMeshEditor::MakeStaticMeshDetails );

		DetailsViewArgs.bAllowSearch			= true;
		DetailsViewArgs.bLockable				= false;
		DetailsViewArgs.bUpdatesFromSelection	= false;
		DetailsViewArgs.NameAreaSettings		= FDetailsViewArgs::HideNameArea;
		DetailsViewArgs.NotifyHook				= this;
		m_DetailsView = PropertyEditorModule.CreateDetailView( DetailsViewArgs );

//		m_DetailsView->RegisterInstancedCustomPropertyLayout( UStaticMesh::StaticClass(), LayoutCustomStaticMeshProperties );
		m_DetailsView->OnFinishedChangingProperties().AddSP( this, &FNaMapEditor::OnFinishedChangingProperties );

		m_DetailsView->SetObject( m_DetailsObject );
	}

	{//! ブロックリスト
		UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
		const TArray<FNaBlockDataAsset*>&	blocks = alib->GetMapBlockList();
		
		for ( auto& it : blocks ){
			TSharedPtr<FBlockDataItem>	ptr;

			ptr	= MakeShareable( new FBlockDataItem() );
			ptr->Asset	= it;
			m_BlockListData.Add( ptr );
		}

		m_BlockList = SNew( SListView<TSharedPtr<FBlockDataItem> > )
			.ListItemsSource( &m_BlockListData )
//			.HeaderRow(RowNamesHeaderRow)
			.OnGenerateRow( this, &FNaMapEditor::MakeBlockDataRowWidget )
			.OnSelectionChanged( this, &FNaMapEditor::OnBlockListRowSelectionChanged );
/*			.OnListViewScrolled(this, &FDataTableEditor::OnRowNamesListViewScrolled)
			.OnSelectionChanged(this, &FDataTableEditor::OnRowSelectionChanged)
			.ScrollbarVisibility(EVisibility::Collapsed)
			.ConsumeMouseWheel(EConsumeMouseWheel::Always)
			.SelectionMode(ESelectionMode::Single)
			.AllowOverscroll(EAllowOverscroll::No);*/
	}

	BuildSubTools();

	//! レイアウト定義
	const TSharedRef<FTabManager::FLayout>	StandaloneDefaultLayout = FTabManager::NewLayout( "NaMapEditor_Layout_v0" )
	->AddArea
	(
		FTabManager::NewPrimaryArea() ->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.1f)
			->SetHideTabWell( true )
			->AddTab( GetToolbarTabId(), ETabState::OpenedTab )
		)
		->Split
		(
			FTabManager::NewSplitter() ->SetOrientation(Orient_Horizontal)
			->SetSizeCoefficient(0.9f)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.6f)
				->AddTab( s_ViewportTabId, ETabState::OpenedTab )
				->SetHideTabWell( true )
			)
			->Split
			(
				FTabManager::NewSplitter() ->SetOrientation(Orient_Vertical)
				->SetSizeCoefficient(0.2f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.7f)
					->AddTab( s_PropertiesTabId, ETabState::OpenedTab )
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.7f)
					->AddTab( s_DetailsTabId, ETabState::OpenedTab )
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.3f)
					->AddTab( s_BlockListTabId, ETabState::OpenedTab )
				)
			)
		)
	);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	const FName identifier = TEXT("NaNRPGEdApp");

	FAssetEditorToolkit::InitAssetEditor( Mode, InitToolkitHost, identifier, StandaloneDefaultLayout, bCreateDefaultToolbar, bCreateDefaultStandaloneMenu, ObjectToEdit );

	ExtendMenu();
	ExtendToolBar();
	RegenerateMenusAndToolbars();
}

//====================================================================================================

//! 
TSharedRef<SDockTab> FNaMapEditor::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	check( Args.GetTabId() == s_ViewportTabId );
	
	TSharedRef<SDockTab> SpawnedTab =
	 SNew(SDockTab)
		.Label( LOCTEXT("StaticMeshViewport_TabTitle", "Viewport") )
		[
			m_Viewport.ToSharedRef()
		];

//	m_Viewport->SetParentTab( SpawnedTab );

	return SpawnedTab;
}

//! 
TSharedRef<SDockTab> FNaMapEditor::SpawnTab_Properties(const FSpawnTabArgs& Args)
{
	check( Args.GetTabId() == s_PropertiesTabId );

	return SNew(SDockTab)
		.Icon( FEditorStyle::GetBrush("StaticMeshEditor.Tabs.Properties") )
		.Label( LOCTEXT("StaticMeshProperties_TabTitle", "Details") )
		[
			m_AssetDetailsView.ToSharedRef()
		];
}

//! 
TSharedRef<SDockTab> FNaMapEditor::SpawnTab_SimpleControl(const FSpawnTabArgs& Args)
{
	check( Args.GetTabId() == s_DetailsTabId );

	return SNew(SDockTab)
		.Icon( FEditorStyle::GetBrush("StaticMeshEditor.Tabs.Properties") )
		.Label( LOCTEXT("StaticMeshProperties_TabTitle", "Details") )
		[
			m_DetailsView.ToSharedRef()
		];
}

//! 
TSharedRef<SDockTab> FNaMapEditor::SpawnTab_BlockList(const FSpawnTabArgs& Args)
{
	check( Args.GetTabId() == s_BlockListTabId );

	return SNew(SDockTab)
		.Label( LOCTEXT("StaticMeshProperties_BlockList", "BlockList") )
		[
			m_BlockList.ToSharedRef()
		];
}

//! 
void FNaMapEditor::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	UProperty*	prop = PropertyChangedEvent.MemberProperty;

	if ( prop ){
		switch ( PropertyChangedEvent.ChangeType ){
		case EPropertyChangeType::ValueSet:
			if ( prop->GetName() == "MapSize" ){
				UpdateFloorSize();
				
				//! 
				m_PrevWorld->ResizeWorld( m_DetailsObject->MapSize );
				//! 
				m_WorldActor->UpdateInvalidateChunk();

				m_CurrentObject->m_MapSize	= m_DetailsObject->MapSize;
				m_CurrentObject->MarkPackageDirty();
			}
			else if ( prop->GetName() == "CreateRandomMap" ){
				if ( m_DetailsObject->CreateRandomMap ){
					//! 
					m_PrevWorld->GenerateRandomTerrain();
					//! 
					m_WorldActor->UpdateInvalidateChunk();
				}
				m_DetailsObject->CreateRandomMap	= false;
			}
			else if ( prop->GetName() == "UseHalfBlock" ){
				int32	size;

				size	= m_DetailsObject->UseHalfBlock ? 127 : 255;
				m_Viewport->SetBlockSizeZ( size );
			}
			break;
		}
	}
}

//! メニュー拡張
void FNaMapEditor::ExtendMenu()
{
}

//! ツールバー拡張
void FNaMapEditor::ExtendToolBar()
{
	struct Local
	{
//		static void FillToolbar(FToolBarBuilder& ToolbarBuilder, TSharedPtr< class STextComboBox > UVChannelCombo, TSharedPtr< class STextComboBox > LODLevelCombo)
		static void FillToolbar( FToolBarBuilder& ToolbarBuilder, FNaMapEditor* owner )
		{
			ToolbarBuilder.BeginSection("Tools");
			{
				ToolbarBuilder.AddToolBarButton( FNaMapEditorCommands::Get().SelectTierTool );
				ToolbarBuilder.AddToolBarButton( FNaMapEditorCommands::Get().SelectMinecraftTool );
			}
			ToolbarBuilder.EndSection();

			ToolbarBuilder.BeginSection("DrawTools");
			{
				ToolbarBuilder.AddToolBarButton( FNaMapEditorCommands::Get().SelectDrawPenTool );
				ToolbarBuilder.AddToolBarButton( FNaMapEditorCommands::Get().SelectDrawBoxTool );
				ToolbarBuilder.AddToolBarButton( FNaMapEditorCommands::Get().SelectDrawFillTool );
				ToolbarBuilder.AddToolBarButton( FNaMapEditorCommands::Get().SelectSelectionTool );
			}
			ToolbarBuilder.EndSection();

			ToolbarBuilder.BeginSection("Command");
			{
				ToolbarBuilder.AddToolBarButton( FNaMapEditorCommands::Get().SetCrossSection );
			}
			ToolbarBuilder.EndSection();
		}
	};

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		m_Viewport->GetCommandList(),
		FToolBarExtensionDelegate::CreateStatic( &Local::FillToolbar, this )
//		FToolBarExtensionDelegate::CreateStatic( &Local::FillToolbar, UVChannelCombo, LODLevelCombo )
		);
	
	AddToolbarExtender(ToolbarExtender);
	
	FNaNRPGEdModule&	AssetToolsModule = FModuleManager::LoadModuleChecked<FNaNRPGEdModule>("NaNRPGEd");

	AddToolbarExtender( AssetToolsModule.GetToolBarExtensibilityManager()->GetAllExtenders( GetToolkitCommands(), GetEditingObjects() ) );
}

//! 補助ツール作成
void FNaMapEditor::BuildSubTools()
{
}

//! 床サイズ更新
void FNaMapEditor::UpdateFloorSize()
{
	//! チャンクサイズからブロックサイズへ変換
	m_Viewport->SetFloorSize( m_DetailsObject->MapSize * 16 );
}

//! 
TSharedRef<ITableRow> FNaMapEditor::MakeBlockDataRowWidget( TSharedPtr<FBlockDataItem> InRowDataPtr, const TSharedRef<STableViewBase>& OwnerTable )
{
	return
		SNew( STableRow<TSharedPtr<FBlockDataItem> >, OwnerTable )
//		.Style(FEditorStyle::Get(), "DataTableEditor.NameListViewRow")
		[
			SNew(SBox)
			.Padding(FMargin(4, 2, 4, 2))
			[
				SNew(SBox)
//				.HeightOverride(InRowDataPtr->DesiredRowHeight)
				[
					SNew( STextBlock )
//					.ColorAndOpacity(this, &FDataTableEditor::GetRowTextColor, InRowDataPtr->RowId)
//					.Text(InRowDataPtr->DisplayName)
//					.Text( FText::FromString( *InRowDataPtr ) )
					.Text( InRowDataPtr->Asset->DisplayName )
//					.HighlightText(this, &FDataTableEditor::GetFilterText)
				]
			]
		];
}

//!
void FNaMapEditor::OnBlockListRowSelectionChanged( TSharedPtr<FBlockDataItem> SelectedItem, ESelectInfo::Type SelectInfo )
{
	m_Viewport->SetCurrentBlock( SelectedItem->Asset );
}

#undef LOCTEXT_NAMESPACE
