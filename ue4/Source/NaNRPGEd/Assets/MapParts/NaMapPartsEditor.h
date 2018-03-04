// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Factories/Factory.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "EditorUndoClient.h"

#include "Assets/NaAssetLibrary.h"
#include "Assets/Map/NaMapPartsAsset.h"

#include "Viewport/SNaMapPartsEditorViewport.h"

#include "NaMapPartsEditor.generated.h"

//! ���쒇��I�u�W�F�N�g�B�����IDetailView�ɃZ�b�g����
//! IStructureDetailView�łł���Ȃ炻���炩�B�B�B
UCLASS()
class UNaMapPartsEditorDetail : public UObject
{
	GENERATED_BODY()

public:
	//! �p�[�c�T�C�Y
	UPROPERTY(EditAnywhere, Category = "MapParts")
	FIntVector	PartsSize;

	//! 
	UPROPERTY(EditAnywhere, Category = "Editor")
	int32		CurrentTier;
};

/**
 * 
 */
class NANRPGED_API FNaMapPartsEditor : public FAssetEditorToolkit, public FGCObject, public FEditorUndoClient, public FNotifyHook
{
	//! 
	struct FBlockDataItem
	{
		FNaBlockDataAsset*	Asset;
	};

public:
	FNaMapPartsEditor();

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
	void	InitEditor( const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UNaMapPartsAsset* ObjectToEdit );

protected:
	//! 
	TSharedRef<SDockTab>	SpawnTab_Viewport(const FSpawnTabArgs& Args);
	//! 
	TSharedRef<SDockTab>	SpawnTab_Properties(const FSpawnTabArgs& Args);
	//! 
	TSharedRef<SDockTab>	SpawnTab_BlockList(const FSpawnTabArgs& Args);

	//!
	void	OnFinishedChangingProperties( const FPropertyChangedEvent& PropertyChangedEvent );

	//! ���j���[�g��
	void	ExtendMenu();
	//! �c�[���o�[�g��
	void	ExtendToolBar();

	//! �⏕�c�[���쐬
	void	BuildSubTools();

	//
	TSharedRef<ITableRow>	MakeBlockDataRowWidget( TSharedPtr<FBlockDataItem> InRowDataPtr, const TSharedRef<STableViewBase>& OwnerTable );
	//!
	void					OnBlockListRowSelectionChanged( TSharedPtr<FBlockDataItem> SelectedItem, ESelectInfo::Type SelectInfo );

protected:
	//! 
	UNaMapPartsAsset*				m_CurrentObject;
	//! 
	UNaMapPartsEditorDetail*	m_DetailsObject;

	//! �r���[�|�[�g
	TSharedPtr<class SNaMapPartsEditorViewport>			m_Viewport;

	//! �ڍ׃p�l��
	TSharedPtr<class IDetailsView>						m_DetailsView;

	//! �u���b�N���X�g
	TSharedPtr<class SListView<TSharedPtr<FBlockDataItem> > >	m_BlockList;
	//! 
	TArray<TSharedPtr<FBlockDataItem> >							m_BlockListData;

	//! 
	TSharedPtr<class SSpinBox<int32> >	m_EditTierSpinBox;


	//! 
	static const FName	s_ViewportTabId;
	static const FName	s_PropertiesTabId;
	static const FName	s_BlockListTabId;
};
