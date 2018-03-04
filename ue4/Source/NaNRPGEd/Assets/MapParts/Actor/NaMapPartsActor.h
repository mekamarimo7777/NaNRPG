// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshComponent.h"

#include "Assets/Map/NaMapPartsAsset.h"

#include "NaMapPartsActor.generated.h"

/**
 * 
 */
UCLASS()
class NANRPGED_API ANaMapPartsActor : public AActor
{
	GENERATED_BODY()

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
	//
	ANaMapPartsActor(const FObjectInitializer& ObjectInitializer);

	//! 
	void	SetMapParts( UNaMapPartsAsset* parts );
	//!
	void	UpdateMesh();

protected:
	//! 
	void	GatherRenderCells();

protected:
	//! 
	UPROPERTY()
	UNaMapPartsAsset*				m_MapParts;

	UPROPERTY()
	UProceduralMeshComponent*	m_ProcMesh;

	//! 一時情報
	TArray<FNaRenderCellWork>	m_RenderCells;
	//! 
	TArray<FNaRenderMaterial>	m_RenderMaterial;
};
