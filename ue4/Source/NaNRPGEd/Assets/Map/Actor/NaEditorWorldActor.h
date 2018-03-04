// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshComponent.h"

#include "Assets/Map/NaMapAsset.h"
#include "Assets/Map/World/NaEditorWorld.h"

#include "Actor/World/NaMapChunkActor.h"

#include "NaEditorWorldActor.generated.h"

/**
 * 
 */
UCLASS()
class NANRPGED_API ANaEditorWorldActor : public AActor
{
	GENERATED_BODY()

public:
	//
	ANaEditorWorldActor(const FObjectInitializer& ObjectInitializer);

	//! 
	void	SetNaWorld( UNaEditorWorld* naw );
	//! 
	void	SetRenderSize( FIntVector size );

	//! 
	void	UpdateChunk( FIntVector chunkPos );
	//! 
	void	UpdateInvalidateChunk();

protected:
	//! 
	void	UpdateChunkActor();
	//! 
	ANaMapChunkActor*	GetChunkActor( const FIntVector& chunkPos );

protected:
	//! エディタワールド
	UPROPERTY()
	UNaEditorWorld*				m_World;
	//! 管理チャンクアクター
	UPROPERTY()
	TArray<ANaMapChunkActor*>	m_ChunkActors;
	//! マップパーツアクター
	//UPROPERTY()
	//TArray<ANaMapPartsActor*>	m_PartsActors;

	//! カレントサイズ
	UPROPERTY()
	FIntVector		m_CurrentSize;
	//! 描画チャンク距離
	UPROPERTY()
	FIntVector		m_RenderSize;
	//! 描画カレント位置
	UPROPERTY()
	FIntVector		m_CurrentPos;
};
