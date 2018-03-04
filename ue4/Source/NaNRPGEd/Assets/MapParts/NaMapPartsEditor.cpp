// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPGEd.h"
#include "NaMapPartsEditor.h"

#include "UnrealEd.h"
#include "TabManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"

#include "SDockTab.h"

#include "Viewport/SNaMapPartsEditorViewport.h"

#include "Actions/NaMapPartsEditorActions.h"

#define LOCTEXT_NAMESPACE "NaNRPGEd"

//====================================================================================================

const FName FNaMapPartsEditor::s_ViewportTabId( TEXT("NaMapPartsEditor_Viewport") );
const FName FNaMapPartsEditor::s_PropertiesTabId( TEXT("NaMapPartsEditor_Properties") );
const FName FNaMapPartsEditor::s_BlockListTabId( TEXT("NaMapPartsEditor_BlockList") );

//====================================================================================================

//! 
FNaMapPartsEditor::FNaMapPartsEditor()
: m_CurrentObject( nullptr )
, m_DetailsObject( nullptr )
{
}

//! 
void FNaMapPartsEditor::AddReferencedObjects( FReferenceCollector& Collector )
{
	Collector.AddReferencedObject( m_CurrentObject );
	Collector.AddReferencedObject( m_DetailsObject );
}

//! 
FName FNaMapPartsEditor::GetToolkitFName() const
{
	return FName("NaMapPartsEditor");
}

//! 
FText FNaMapPartsEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "NaMapPartsEditor");
}

//! 
FString FNaMapPartsEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "NaMapParts").ToString();
}

//! 
FLinearColor FNaMapPartsEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor( 0.3f, 0.2f, 0.5f, 0.5f );
}

//! 
void FNaMapPartsEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_StaticMeshEditor", "Static Mesh Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner( s_ViewportTabId, FOnSpawnTab::CreateSP(this, &FNaMapPartsEditor::SpawnTab_Viewport) )
		.SetDisplayName( LOCTEXT("ViewportTab", "Viewport") )
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));

	InTabManager->RegisterTabSpawner( s_PropertiesTabId, FOnSpawnTab::CreateSP(this, &FNaMapPartsEditor::SpawnTab_Properties) )
		.SetDisplayName( LOCTEXT("PropertiesTab", "Details") )
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner( s_BlockListTabId, FOnSpawnTab::CreateSP(this, &FNaMapPartsEditor::SpawnTab_BlockList) )
		.SetDisplayName( LOCTEXT("BlockListTab", "BlockList") )
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

//! 
void FNaMapPartsEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner( s_ViewportTabId );
	InTabManager->UnregisterTabSpawner( s_PropertiesTabId );
	InTabManager->UnregisterTabSpawner( s_BlockListTabId );
}

//! エディタ初期化
void FNaMapPartsEditor::InitEditor( const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UNaMapPartsAsset* ObjectToEdit )
{
	m_CurrentObject	= ObjectToEdit;

	m_DetailsObject	= NewObject<UNaMapPartsEditorDetail>();
	if ( m_DetailsObject ){
		m_DetailsObject->PartsSize	= m_CurrentObject->Size;
	}

	//! 
	FNaMapPartsEditorCommands::Register();

	{//! メインビューポート
		m_Viewport = SNew( SNaMapPartsEditorViewport );
//			.StaticMeshEditor(SharedThis(this))
//			.ObjectToEdit(ObjectToEdit);

		m_Viewport->SpawnPartsActor();
		m_Viewport->SetMapParts( ObjectToEdit );
	}

	{//! 詳細パネル（C#でのPropertyGrid）
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
		m_DetailsView->OnFinishedChangingProperties().AddSP( this, &FNaMapPartsEditor::OnFinishedChangingProperties );

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
			.OnGenerateRow( this, &FNaMapPartsEditor::MakeBlockDataRowWidget )
			.OnSelectionChanged( this, &FNaMapPartsEditor::OnBlockListRowSelectionChanged );
/*			.OnListViewScrolled(this, &FDataTableEditor::OnRowNamesListViewScrolled)
			.OnSelectionChanged(this, &FDataTableEditor::OnRowSelectionChanged)
			.ScrollbarVisibility(EVisibility::Collapsed)
			.ConsumeMouseWheel(EConsumeMouseWheel::Always)
			.SelectionMode(ESelectionMode::Single)
			.AllowOverscroll(EAllowOverscroll::No);*/
	}

	BuildSubTools();

	//! レイアウト定義
	const TSharedRef<FTabManager::FLayout>	StandaloneDefaultLayout = FTabManager::NewLayout( "NaMapPartsEditor_Layout_v0" )
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

/*
	FReimportManager::Instance()->OnPostReimport().AddRaw(this, &FStaticMeshEditor::OnPostReimport);

	// Support undo/redo
	ObjectToEdit->SetFlags( RF_Transactional );

	GEditor->RegisterForUndo( this );

	// Register our commands. This will only register them if not previously registered
	FStaticMeshEditorCommands::Register();

	// Register to be notified when an object is reimported.
	GEditor->OnObjectReimported().AddSP(this, &FStaticMeshEditor::OnObjectReimported);

	BindCommands();

	Viewport = SNew(SStaticMeshEditorViewport)
		.StaticMeshEditor(SharedThis(this))
		.ObjectToEdit(ObjectToEdit);

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
	
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.NotifyHook = this;

	StaticMeshDetailsView = PropertyEditorModule.CreateDetailView( DetailsViewArgs );

	FOnGetDetailCustomizationInstance LayoutCustomStaticMeshProperties = FOnGetDetailCustomizationInstance::CreateSP( this, &FStaticMeshEditor::MakeStaticMeshDetails );
	StaticMeshDetailsView->RegisterInstancedCustomPropertyLayout( UStaticMesh::StaticClass(), LayoutCustomStaticMeshProperties );

	SetEditorMesh(ObjectToEdit);

	BuildSubTools();

	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout( "Standalone_StaticMeshEditor_Layout_v4" )
	->AddArea
	(
		FTabManager::NewPrimaryArea() ->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.1f)
			->SetHideTabWell( true )
			->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
		)
		->Split
		(
			FTabManager::NewSplitter() ->SetOrientation(Orient_Horizontal)
			->SetSizeCoefficient(0.9f)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.6f)
				->AddTab(ViewportTabId, ETabState::OpenedTab)
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
					->AddTab(PreviewSceneSettingsTabId, ETabState::OpenedTab)
					->AddTab(PropertiesTabId, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.3f)
					->AddTab(SocketManagerTabId, ETabState::OpenedTab)
					->AddTab(CollisionTabId, ETabState::ClosedTab)
				)
			)
		)
	);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor( Mode, InitToolkitHost, StaticMeshEditorAppIdentifier, StandaloneDefaultLayout, bCreateDefaultToolbar, bCreateDefaultStandaloneMenu, ObjectToEdit );
	
	ExtendMenu();
	ExtendToolBar();
	RegenerateMenusAndToolbars();
*/
}

//====================================================================================================

//! 
TSharedRef<SDockTab> FNaMapPartsEditor::SpawnTab_Viewport(const FSpawnTabArgs& Args)
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
TSharedRef<SDockTab> FNaMapPartsEditor::SpawnTab_Properties(const FSpawnTabArgs& Args)
{
	check( Args.GetTabId() == s_PropertiesTabId );

	return SNew(SDockTab)
		.Icon( FEditorStyle::GetBrush("StaticMeshEditor.Tabs.Properties") )
		.Label( LOCTEXT("StaticMeshProperties_TabTitle", "Details") )
		[
			m_DetailsView.ToSharedRef()
		];
}

//! 
TSharedRef<SDockTab> FNaMapPartsEditor::SpawnTab_BlockList(const FSpawnTabArgs& Args)
{
	check( Args.GetTabId() == s_BlockListTabId );

	return SNew(SDockTab)
		.Label( LOCTEXT("StaticMeshProperties_BlockList", "BlockList") )
		[
			m_BlockList.ToSharedRef()
		];
}

//! 
void FNaMapPartsEditor::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	UProperty*	prop = PropertyChangedEvent.MemberProperty;

	if ( prop ){
		switch ( PropertyChangedEvent.ChangeType ){
		case EPropertyChangeType::ValueSet:
			if ( prop->GetName() == "PartsSize" ){
				ANaMapPartsActor*	actor = m_Viewport->GetPartsActor();

//				m_EditTierSpinBox->SetMaxValue( m_DetailsObject->PartsSize.Z );
				m_Viewport->SetFloorSize( m_DetailsObject->PartsSize );

				m_CurrentObject->Resize( m_DetailsObject->PartsSize );
				m_CurrentObject->MarkPackageDirty();

				actor->UpdateMesh();
			}
			else if ( prop->GetName() == "CurrentTier" ){
				int32	tier = FMath::Clamp( m_DetailsObject->CurrentTier, 0, m_CurrentObject->Size.Z );

				m_DetailsObject->CurrentTier	= tier;
				m_Viewport->SetCurrentTier( m_DetailsObject->CurrentTier );
			}
			break;
		}
	}
/*	if (PropertyChangedEvent.Property)
	{
		FGraphPanelSelectionSet CurrentSelection = GetSelectedNodes();
		if (CurrentSelection.Num() == 1)
		{
			for (FGraphPanelSelectionSet::TConstIterator It(CurrentSelection); It; ++It)
			{
				UEnvironmentQueryGraphNode_Test* TestNode = Cast<UEnvironmentQueryGraphNode_Test>(*It);
				UEnvironmentQueryGraphNode_Option* ParentNode = TestNode ? Cast<UEnvironmentQueryGraphNode_Option>(TestNode->ParentNode) : nullptr;

				if (ParentNode)
				{
					ParentNode->CalculateWeights();
					break;
				}
			}
		}
	}*/
}

//! 
void FNaMapPartsEditor::ExtendMenu()
{
}

//! ツールバー拡張
void FNaMapPartsEditor::ExtendToolBar()
{
	struct Local
	{
//		static void FillToolbar(FToolBarBuilder& ToolbarBuilder, TSharedPtr< class STextComboBox > UVChannelCombo, TSharedPtr< class STextComboBox > LODLevelCombo)
		static void FillToolbar( FToolBarBuilder& ToolbarBuilder, FNaMapPartsEditor* owner )
		{
			ToolbarBuilder.BeginSection("Tools");
			{
//				ToolbarBuilder.AddToolBarButton( FNaMapPartsEditorCommands::Get().SelectTierTool );
//				ToolbarBuilder.AddWidget( owner->m_EditTierSpinBox.ToSharedRef() );
				ToolbarBuilder.AddToolBarButton( FNaMapPartsEditorCommands::Get().SelectMinecraftTool );
			}
			ToolbarBuilder.EndSection();

			ToolbarBuilder.BeginSection("DrawTools");
			{
				ToolbarBuilder.AddToolBarButton( FNaMapPartsEditorCommands::Get().SelectDrawPenTool );
				ToolbarBuilder.AddToolBarButton( FNaMapPartsEditorCommands::Get().SelectDrawBoxTool );
				ToolbarBuilder.AddToolBarButton( FNaMapPartsEditorCommands::Get().SelectDrawFillTool );
				ToolbarBuilder.AddToolBarButton( FNaMapPartsEditorCommands::Get().SelectSelectionTool );
			}
			ToolbarBuilder.EndSection();

			ToolbarBuilder.BeginSection("Command");
			{
				ToolbarBuilder.AddToolBarButton( FNaMapPartsEditorCommands::Get().SetCrossSection );
			}
			ToolbarBuilder.EndSection();

/*			ToolbarBuilder.BeginSection("Realtime");
			{
				ToolbarBuilder.AddToolBarButton(FEditorViewportCommands::Get().ToggleRealTime);
			}
			ToolbarBuilder.EndSection();
/*	
			ToolbarBuilder.BeginSection("Command");
			{
				ToolbarBuilder.AddToolBarButton( FStaticMeshEditorCommands::Get().SetShowSockets );
				ToolbarBuilder.AddToolBarButton(FStaticMeshEditorCommands::Get().SetShowWireframe);
				ToolbarBuilder.AddToolBarButton(FStaticMeshEditorCommands::Get().SetShowVertexColor);
				ToolbarBuilder.AddToolBarButton(FStaticMeshEditorCommands::Get().SetShowGrid);
				ToolbarBuilder.AddToolBarButton(FStaticMeshEditorCommands::Get().SetShowBounds);
				ToolbarBuilder.AddToolBarButton(FStaticMeshEditorCommands::Get().SetShowCollision);
				ToolbarBuilder.AddToolBarButton(FStaticMeshEditorCommands::Get().SetShowPivot);
				ToolbarBuilder.AddToolBarButton(FStaticMeshEditorCommands::Get().SetShowNormals);
				ToolbarBuilder.AddToolBarButton(FStaticMeshEditorCommands::Get().SetShowTangents);
				ToolbarBuilder.AddToolBarButton(FStaticMeshEditorCommands::Get().SetShowBinormals);
				ToolbarBuilder.AddToolBarButton(FStaticMeshEditorCommands::Get().SetShowVertices);
				ToolbarBuilder.AddToolBarButton(FStaticMeshEditorCommands::Get().SetDrawUVs);
				ToolbarBuilder.AddToolBarButton(FStaticMeshEditorCommands::Get().SetDrawAdditionalData);
			}
			ToolbarBuilder.EndSection();
	
			ToolbarBuilder.BeginSection("UV");
			{
				ToolbarBuilder.AddWidget(UVChannelCombo.ToSharedRef());
			}
			ToolbarBuilder.EndSection();
	
			ToolbarBuilder.BeginSection("Camera");
			{
				ToolbarBuilder.AddToolBarButton(FStaticMeshEditorCommands::Get().ResetCamera);
			}
			ToolbarBuilder.EndSection();
	
			ToolbarBuilder.BeginSection("LOD");
			{
				ToolbarBuilder.AddWidget(LODLevelCombo.ToSharedRef());
			}
			ToolbarBuilder.EndSection();*/
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
void FNaMapPartsEditor::BuildSubTools()
{
	//! 
//	m_EditTierSpinBox	= SNew( SSpinBox<int32> )
//		.MaxValue( m_CurrentObject->Size.Z )
//		.OnValueChanged( m_Viewport.Get(), &SNaMapPartsEditorViewport::OnCurrentTierChanged );
//		.OptionsSource(&UVChannels)
//		.OnSelectionChanged(this, &FStaticMeshEditor::ComboBoxSelectionChanged)
//		.IsEnabled( FSlateApplication::Get().GetNormalExecutionAttribute() );
}

//! 
TSharedRef<ITableRow> FNaMapPartsEditor::MakeBlockDataRowWidget( TSharedPtr<FBlockDataItem> InRowDataPtr, const TSharedRef<STableViewBase>& OwnerTable )
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
void FNaMapPartsEditor::OnBlockListRowSelectionChanged( TSharedPtr<FBlockDataItem> SelectedItem, ESelectInfo::Type SelectInfo )
{
	m_Viewport->SetCurrentBlock( SelectedItem->Asset );
}

#undef LOCTEXT_NAMESPACE
