// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "ProceduralMeshComponent.h"

#include "Assets/World/NaBlockDataTable.h"
#include "World/NaWorld.h"

#include "NaMapChunkActor.generated.h"

class ANaWorldActor;

//! 描画用ワーク
struct FNaRenderCellWork
{
	//! セル座標
	uint8	X;
	uint8	Y;
	uint8	Z;

	//! 描画面
	uint8	Faces;
	//! 
	//int8	MaterialIdx;

	//! セル情報
	FNaMapPartsCell*	Cell;
};

//! 描画レイヤー
USTRUCT()
struct FNaChunkRenderLayer
{
	GENERATED_BODY()

	//! 
	uint8		ID;
	//!
	UPROPERTY()
	UNaChunk*	Chunk;

	//! メッシュ
	UPROPERTY()
	UProceduralMeshComponent*	ProcMesh;
	//! 
	TArray<FNaRenderCellWork>	RenderCells;

	//! 
	bool	Invalidate;

	FNaChunkRenderLayer()
	: ID(-1)
	, Chunk( nullptr )
	, ProcMesh( nullptr )
	, Invalidate( false )
	{
	}
};

// マップチャンクアクター
// 役割：チャンクデータの生成・描画、更新時の処理
UCLASS()
class NANRPG_API ANaMapChunkActor : public AActor
{
	GENERATED_BODY()

	//! 
	struct FNaRenderMaterial
	{
		//! セルインデックス
		TArray<int16>				CellIndices;
		//! マテリアル
		FNaBlockMaterialAsset*		MaterialAsset;

		//! 
		UMaterialInterface*			Material;

		// メッシュデータ //
		TArray<FVector>				Vertices;
		TArray<int32>				Indices;
		TArray<FVector>				Normals;
		TArray<FVector2D>			UVs;
		TArray<FColor>				Colors;
		TArray<FProcMeshTangent>	Tangents;

		FNaRenderMaterial()
		: MaterialAsset( nullptr )
		, Material( nullptr )
		{
		}
	};

public:
	//! コンストラクタ
	ANaMapChunkActor( const FObjectInitializer& ObjectInitializer );

	// 
	virtual void BeginPlay() override;
	
	// 
	virtual void Tick( float DeltaSeconds ) override;

	//! ワールドアクタ
	void	SetWorldActor( ANaWorldActor* actor )	{ m_WorldActor = actor; }

	//! チャンク設定
	void	SetChunk( UNaChunk* chunk );
	//! チャンクメッシュ更新
	void	UpdateChunkMesh( int32 layer );

	//! 
	void	SetCollidable( bool value )	{ m_Collidable = value; }

protected:
	//! 
	void	GatherRenderCells( int32 layer );
	
protected:
	//! ワールドアクタ
	UPROPERTY()
	ANaWorldActor*	m_WorldActor;

	//! メッシュ
	UPROPERTY()
	UProceduralMeshComponent*	m_ProcMesh;
	UPROPERTY()
	TArray<FNaChunkRenderLayer>	m_Layer;
	//! 
	bool						m_Collidable;
	//! 
	bool						m_Invalidate;

	//! 表示チャンク
	UPROPERTY()
	UNaChunk*	m_Chunk;

	//! 一時情報
	TArray<FNaRenderCellWork>	m_RenderCells;
	//! 
	TArray<FNaRenderMaterial>	m_RenderMaterial;
};
