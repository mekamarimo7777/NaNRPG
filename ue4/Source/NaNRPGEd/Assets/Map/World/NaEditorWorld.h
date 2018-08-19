// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Assets/World/NaMapAsset.h"

#include "World/NaWorld.h"

#include "NaEditorWorld.generated.h"

/**
 * 
 */
UCLASS()
class NANRPGED_API UNaEditorWorld : public UNaWorld
{
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	UNaEditorWorld();

	//! �}�b�v�A�Z�b�g����\�z
	void		SetupFromMap( UNaMapAsset* asset );
	//! �}�b�v�T�C�Y�ύX
	void		ResizeWorld( FIntVector size );
	//! 
	FIntVector	GetWorldSize();

	//! �u���b�N�X�V
	virtual void	SetBlock( FIntVector pos, FNaWorldBlockWork& block ) override;

	//! �ĕ]��
	virtual void	Evaluate() override;

	//! �����_���n�`����
	void	GenerateRandomTerrain();

	//! ���[�W�����f�[�^�ǂݍ���
	virtual bool	ReadRegionData( FIntVector pos, TArray<uint8>& outVal ) override;
	//! ���[�W�����f�[�^��������
	virtual bool	WriteRegionData( FIntVector pos, const TArray<uint8>& inVal ) override;

protected:
	//! 
	UNaChunk*	CreateChunk( FIntVector pos );
	//! �`�����N�擾
	UNaChunk*	FindChunk( FIntVector pos );

	//! ���[�W�����擾
	UNaRegion*	FindRegion( FIntVector pos );
	//! ���[�W�����I�[�v��
	UNaRegion*	OpenRegion( FIntVector pos );

public:
	
protected:
	//! �Ǘ��A�Z�b�g
	UNaMapAsset*	m_MapAsset;

	//! �������[�W�����f�[�^
	TMap<FIntVector, TArray<uint8> >	m_RegionData;

};
