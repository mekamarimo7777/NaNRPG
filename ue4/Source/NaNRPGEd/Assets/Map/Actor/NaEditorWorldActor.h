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
	//! �G�f�B�^���[���h
	UPROPERTY()
	UNaEditorWorld*				m_World;
	//! �Ǘ��`�����N�A�N�^�[
	UPROPERTY()
	TArray<ANaMapChunkActor*>	m_ChunkActors;
	//! �}�b�v�p�[�c�A�N�^�[
	//UPROPERTY()
	//TArray<ANaMapPartsActor*>	m_PartsActors;

	//! �J�����g�T�C�Y
	UPROPERTY()
	FIntVector		m_CurrentSize;
	//! �`��`�����N����
	UPROPERTY()
	FIntVector		m_RenderSize;
	//! �`��J�����g�ʒu
	UPROPERTY()
	FIntVector		m_CurrentPos;
};
