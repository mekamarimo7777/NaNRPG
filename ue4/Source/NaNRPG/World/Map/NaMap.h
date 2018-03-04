// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "World/NaChunk.h"

#include "Assets/Map/NaMapAsset.h"

#include "NaMap.generated.h"

/**
 * ���ԃ}�b�v�C���X�^���X�i�\�����f�[�^���琶���ς݂̃`�����N�ō\�������j
 */
UCLASS()
class NANRPG_API UNaMap : public UObject
{
	GENERATED_BODY()
	
public:
	//! �}�b�v����
	void	Create( UNaWorld* naw, int32 uid, FIntVector pos, const UNaMapAsset* asset );

	//! �}�b�v�ۑ�
	void	SaveMap( FString path );
	//! �}�b�v�ǂݍ���
	void	LoadMap( FString path, int32 id );

	//!	�`�����N�擾
	UNaChunk*	GetChunk( FIntVector cpos );
	//! �}�b�v�N���[�Y
	void		CloseMap();

	//! 
	virtual void	Serialize( FArchive& ar ) override;

	//! @deprecated �}�b�v�A�Z�b�g����C���X�^���X����
	void	Instantiate( UNaMapAsset* src );
	//
	void	CopyPartsToChunk( FNaMapPartsEntry* parts, UNaChunk* chunk );
	
	//! 
	void		SetLocation(FIntVector pos)	{ m_MapLocation = pos; }
	//! 
	FIntVector	GetLocation() const			{ return m_MapLocation; }

protected:
	//! �}�b�vID
	int32		m_MapID;
	//! �}�b�v�A�Z�b�g
	int32		m_AssetID;
	//! �}�b�v�z�u���W
	FIntVector	m_MapLocation;
	//! �}�b�v�T�C�Y�i�`�����N�P�ʁj
	FIntVector	m_MapSize;

	// �`�����N��� //
	UPROPERTY()
	TArray<UNaChunk*>				m_Chunks;		// ���̕ێ��p //
	TMap<FIntVector, UNaChunk*>		m_ChunkMap;
	// �`�����N�L���b�V�� //
	TMap<FIntVector, TArray<uint8>>	m_ChunkCache;
};
