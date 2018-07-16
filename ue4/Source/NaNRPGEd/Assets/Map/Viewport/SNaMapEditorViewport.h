// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AdvancedPreviewScene.h"
#include "SEditorViewport.h"
#include "Editor/UnrealEd/Public/SCommonEditorViewportToolbarBase.h"

#include "NaMapEditorViewportClient.h"

#include "Assets/World/NaMapAsset.h"

#include "Assets/MapParts/Actor/NaMapPartsActor.h"
#include "Assets/MapParts/Viewport/SNaMapPartsEditorViewport.h"

#include "Assets/Map/World/NaEditorWorld.h"
#include "Assets/Map/Actor/NaEditorWorldActor.h"


/*
//! 
enum class ENaMapEditTool
{
	//! 
	Tier,
	//! 
	MinecraftLike,
	
	ENUM_MAX
};

//! 
enum class ENaMapDrawTool
{
	//! 
	Pen,
	//! 
	Box,
	//! 
	Fill,
	//! 
	Selection,
	
	ENUM_MAX
};
*/

/**
 * StaticMesh Editor Preview viewport widget
 */
class SNaMapEditorViewport : public SEditorViewport, public FGCObject, public ICommonEditorViewportToolbarInfoProvider
{
public:
	SNaMapEditorViewport();
	virtual ~SNaMapEditorViewport();

	// FGCObject interface
	virtual void AddReferencedObjects( FReferenceCollector& Collector ) override;
	// End of FGCObject interface

	// ICommonEditorViewportToolbarInfoProvider interface
	virtual TSharedRef<SEditorViewport>	GetViewportWidget() override;
	virtual TSharedPtr<FExtender>		GetExtenders() const override;
	virtual void						OnFloatingButtonClicked() override;
	// End of ICommonEditorViewportToolbarInfoProvider interface

	//! 
	void	SetEditorWorld( UNaEditorWorld* naw );
	//! 
	void	SetFloorSize( FIntVector size );
	//! 
	void	SetCurrentBlock( FNaBlockDataAsset* block )		{ m_CurrentBlock = block; }

	//! ブロック縦サイズ設定
	void	SetBlockSizeZ( int32 z )	{ m_BlockSizeZ	= z; }

	//! 
	ANaEditorWorldActor*	GetWorldActor() const	{ return m_WorldActor; }

	//! 
	void	OnHoverMouse( FVector& pos, FVector& dir );
	//! 
	void	OnClickViewport( FVector& pos, FVector& dir, FKey key );

protected:
	//! 
	virtual void	Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) override;

	/** SEditorViewport interface */
	virtual TSharedRef<FEditorViewportClient>	MakeEditorViewportClient() override;

	//!
	virtual void	BindCommands() override;

	//! 
	void	SelectTierTool();
	//! 
	bool	IsSelectTierToolChecked() const;
	
	//! 
	void	SelectMinecraftTool();
	//! 
	bool	IsSelectMinecraftToolChecked() const;


public:

protected:
	//! 
	TSharedPtr<FNaMapEditorViewportClient>	m_ViewportClient;
	//!
	FAdvancedPreviewScene	m_PreviewScene;

	//!
	UNaEditorWorld*			m_World;
	//! 
	ANaEditorWorldActor*	m_WorldActor;

	//! 
	ENaMapPartsEditTool		m_EditTool;
	//! 
	ENaMapPartsDrawTool		m_DrawTool;
	//! 
	FIntVector				m_CurrentCellPos;
	//!
	FNaBlockDataAsset*		m_CurrentBlock;

	//! 配置ブロック縦サイズ
	int32	m_BlockSizeZ;
};
