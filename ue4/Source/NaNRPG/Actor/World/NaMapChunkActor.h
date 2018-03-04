// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "ProceduralMeshComponent.h"

#include "Assets/Map/NaBlockDataTable.h"
#include "World/NaWorld.h"

#include "NaMapChunkActor.generated.h"

class ANaWorldActor;

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

//! �`�惌�C���[
USTRUCT()
struct FNaChunkRenderLayer
{
	GENERATED_BODY()

	//! 
	uint8		ID;
	//!
	UPROPERTY()
	UNaChunk*	Chunk;

	//! ���b�V��
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

// �}�b�v�`�����N�A�N�^�[
// �����F�`�����N�f�[�^�̐����E�`��A�X�V���̏���
UCLASS()
class NANRPG_API ANaMapChunkActor : public AActor
{
	GENERATED_BODY()

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
	//! �R���X�g���N�^
	ANaMapChunkActor( const FObjectInitializer& ObjectInitializer );

	// 
	virtual void BeginPlay() override;
	
	// 
	virtual void Tick( float DeltaSeconds ) override;

	//! ���[���h�A�N�^
	void	SetWorldActor( ANaWorldActor* actor )	{ m_WorldActor = actor; }

	//! �`�����N�ݒ�
	void	SetChunk( UNaChunk* chunk );
	//! �`�����N���b�V���X�V
	void	UpdateChunkMesh( int32 layer );

	//! 
	void	SetCollidable( bool value )	{ m_Collidable = value; }

protected:
	//! 
	void	GatherRenderCells( int32 layer );
	
protected:
	//! ���[���h�A�N�^
	UPROPERTY()
	ANaWorldActor*	m_WorldActor;

	//! ���b�V��
	UPROPERTY()
	UProceduralMeshComponent*	m_ProcMesh;
	UPROPERTY()
	TArray<FNaChunkRenderLayer>	m_Layer;
	//! 
	bool						m_Collidable;
	//! 
	bool						m_Invalidate;

	//! �\���`�����N
	UPROPERTY()
	UNaChunk*	m_Chunk;

	//! �ꎞ���
	TArray<FNaRenderCellWork>	m_RenderCells;
	//! 
	TArray<FNaRenderMaterial>	m_RenderMaterial;
};
