// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "NaChunk.h"

#include "NaRegion.generated.h"

class UNaWorld;
class UNaMap;

/**
 * 
 */
UCLASS()
class NANRPG_API UNaRegion : public UObject
{
	GENERATED_BODY()
	
public:
	//!
	void	Initialize( UNaWorld* world, FIntVector rpos );

	//! ���[�W�����I�[�v��
	void	Open();
	//! ���[�W�����N���[�Y
	void	Close();





	//
	bool	OpenRegion( const FString& path, FIntVector rpos );
	//
	bool	CreateRegion( const FString& path, FIntVector rpos );
	//
	void	CloseRegion();

	//
	UNaChunk*	GetChunk( FIntVector cpos );
	//
	UNaChunk*	CreateChunk( FIntVector cpos );
	// �`�����N�N���[�Y�i�L���b�V���Ɉړ��j
	void		CloseChunk( UNaChunk* chunk );
	void		CloseChunk( FIntVector cpos );

	//
	void	RecalcVisibleFaces();

	// �V���A���C�Y //
	virtual void	Serialize( FArchive& ar ) override;

	//
	FORCEINLINE UNaWorld*	GetNaWorld() const			{return m_World;}
	//
	FORCEINLINE FIntVector	GetRegionPos() const		{return m_Location;}
	// 
	FORCEINLINE int32		GetActiveChunkCount() const	{return m_Chunks.Num();}

	//
	void	ConnectChunk( UNaChunk* chunk );
	//
	void	DisconnectChunk( UNaChunk* chunk );
	
protected:
	//! ���[���h
	UPROPERTY(Transient)
	UNaWorld*	m_World;

	//! �t�@�C���p�X
	FString		m_FilePath;
	//! ���[�W�������W
	FIntVector	m_Location;

	// ���[�h���̃`�����N��� //
	UPROPERTY(Transient)
	TArray<UNaChunk*>			m_Chunks;
	TMap<FIntVector, UNaChunk*>	m_ChunkMap;

	// �`�����N�L���b�V���i���k�o�C�g�f�[�^�j //
	// �A�����[�h�ΏۂɂȂ����`�����N�͈�U������Ɋi�[����A���[�W�����폜���Ƀt�@�C���֏������܂�� //
	TMap<FIntVector, TArray<uint8> >	m_ChunkCache;

	UPROPERTY(Transient)
	TArray<UNaMap*>				m_MapData;		// �Q�ƃ}�b�v
};
