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

	//! �`��p���[�N
	struct FNaRenderCellWork
	{
		//! �Z�����W
		uint8	X;
		uint8	Y;
		uint8	Z;

		//! �`���
		uint8	Faces;
		//! 
		//int8	MaterialIdx;

		//! �Z�����
		FNaMapPartsCell*	Cell;
	};

	//! 
	struct FNaRenderMaterial
	{
		//! �Z���C���f�b�N�X
		TArray<int16>				CellIndices;
		//! �}�e���A��
		FNaBlockMaterialAsset*		MaterialAsset;

		//! 
		UMaterialInterface*			Material;

		// ���b�V���f�[�^ //
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

	//! �ꎞ���
	TArray<FNaRenderCellWork>	m_RenderCells;
	//! 
	TArray<FNaRenderMaterial>	m_RenderMaterial;
};
