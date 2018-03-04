// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Factories/Factory.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "EditorUndoClient.h"

#include "Assets/NaAssetLibrary.h"
#include "Assets/Map/NaMapAsset.h"
#include "Assets/Map/World/NaEditorWorld.h"
#include "Assets/Map/Actor/NaEditorWorldActor.h"

#include "Viewport/SNaMapEditorViewport.h"

#include "NaMapEditor.generated.h"

//! 操作仲介オブジェクト。これをIDetailViewにセットする
UCLASS()
class UNaMapEditorDetail : public UObject
{
	GENERATED_BODY()

public:
	//! マップサイズ
	UPROPERTY(EditAnywhere, Category = "Editor")
	FIntVector	MapSize;
	//! 
	UPROPERTY(EditAnywhere, Category = "Editor")
	bool		CreateRandomMap;

	//! ハーフブロック設定
	UPROPERTY(EditAnywhere, Category = "SimpleTools")
	bool		UseHalfBlock;
};

/**
 * 
 */
class NANRPGED_API FNaMapEditor : public FAssetEditorToolkit, public FGCObject, public FEditorUndoClient, public FNotifyHook
{
	//! 
	struct FBlockDataItem
	{
		FNaBlockDataAsset*	Asset;
	};

public:
	FNaMapEditor();

	//~ Begin FGCObject Interface
	virtual void	AddReferencedObjects( FReferenceCollector& Collector ) override;
	//~ End FGCObject Interface

	/** IToolkit interface */
	virtual FName	GetToolkitFName() const override;
	virtual FText	GetBaseToolkitName() const override;
	virtual FString	GetWorldCentricTabPrefix() const override;

	virtual FLinearColor	GetWorldCentricTabColorScale() const override;
	/** End IToolkit interface */

	//! 
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	//! 
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

	//!
	void	InitEditor( const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UNaMapAsset* ObjectToEdit );

protected:
	//! 
	TSharedRef<SDockTab>	SpawnTab_Viewport(const FSpawnTabArgs& Args);
	//! 
	TSharedRef<SDockTab>	SpawnTab_SimpleControl(const FSpawnTabArgs& Args);
	//! 
	TSharedRef<SDockTab>	SpawnTab_Properties(const FSpawnTabArgs& Args);
	//! 
	TSharedRef<SDockTab>	SpawnTab_BlockList(const FSpawnTabArgs& Args);

	//!
	void	OnFinishedChangingProperties( const FPropertyChangedEvent& PropertyChangedEvent );

	//! メニュー拡張
	void	ExtendMenu();
	//! ツールバー拡張
	void	ExtendToolBar();

	//! 補助ツール作成
	void	BuildSubTools();

	//! 床サイズ更新
	void	UpdateFloorSize();

	//
	TSharedRef<ITableRow>	MakeBlockDataRowWidget( TSharedPtr<FBlockDataItem> InRowDataPtr, const TSharedRef<STableViewBase>& OwnerTable );
	//!
	void					OnBlockListRowSelectionChanged( TSharedPtr<FBlockDataItem> SelectedItem, ESelectInfo::Type SelectInfo );

protected:
	//! 編集オブジェクト
	UNaMapAsset*			m_CurrentObject;
	//! 簡易編集オブジェクト
	UNaMapEditorDetail*		m_DetailsObject;

	//! 
	UNaEditorWorld*			m_PrevWorld;
	//! 
	ANaEditorWorldActor*	m_WorldActor;

	//! ビューポート
	TSharedPtr<class SNaMapEditorViewport>	m_Viewport;

	//! 詳細パネル
	TSharedPtr<class IDetailsView>			m_AssetDetailsView;
	//! 簡易編集パネル
	TSharedPtr<class IDetailsView>			m_DetailsView;

	//! ブロックリスト
	TSharedPtr<class SListView<TSharedPtr<FBlockDataItem> > >	m_BlockList;
	//! 
	TArray<TSharedPtr<FBlockDataItem> >							m_BlockListData;

	//! タブID定義
	static const FName	s_ViewportTabId;
	static const FName	s_PropertiesTabId;
	static const FName	s_DetailsTabId;
	static const FName	s_BlockListTabId;
};
