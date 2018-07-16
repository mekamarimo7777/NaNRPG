// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Misc/NaEnums.h"

#include "NaRegion.h"
#include "Map/NaMap.h"
#include "Generation/NaWorldGenerator.h"

#include "World/NaWorldDefinition.h"

#include "NaWorld.generated.h"

class ANaWorldActor;
class UNaWorldAsset;
class UNaWorldManager;
class UNaEntity;
class UNaEntityPlayer;
class UNaTurnActionComponent;
class UNaEventManager;

USTRUCT()
struct FNaMapEntry
{
	GENERATED_BODY()

	//! �ŗLID
	UPROPERTY()
	int32		MapID;
	//! �z�u�`�����N���W
	UPROPERTY()
	FIntVector	Location;
	//! �z�u�͈�
	UPROPERTY()
	FIntVector	Range;
};

//! 
class INaWorldDataAccessor
{
	//! ���[�W�����f�[�^�ǂݍ���
	virtual bool	ReadRegionData( FIntVector pos, TArray<uint8>& outVal )			{ return false; }
	//! ���[�W�����f�[�^��������
	virtual bool	WriteRegionData( FIntVector pos, const TArray<uint8>& inVal )	{ return false; }
};

/**
 * ���[���h�C���X�^���X
 */
UCLASS()
class NANRPG_API UNaWorld : public UObject, public INaWorldDataAccessor
{
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	UNaWorld();

	//! �g�p�J�n����
	virtual void	Setup( ANaWorldActor* actor );
	//! �X�V
	virtual void	Update( float DeltaTime );

	//! �V�K�쐬
	virtual bool	CreateWorld( FName uid, FName assetID );
	//! �J��
	bool			OpenWorld( int32 dataID );
	//! ����
	void			CloseWorld( bool isSave = true );

	//! �ŗLID�擾
	FName	GetUID() const		{ return m_UID; }
	//! �A�Z�b�gID�擾
	FName	GetAssetID() const	{ return m_AssetID; }
	//! �f�[�^ID�擾
	uint32	GetDataID() const	{ return m_DataID; }
	//! �N���[�Y����
	bool	IsClosed() const	{ return false; }



	//! ���[���h�W�F�l���[�^�擾
	UNaWorldGenerator*	GetGenerator() const	{ return m_Generator; }

	// �V���A���C�Y //
	virtual void	Serialize( FArchive& ar ) override;

	//! 
	void		SetChunkRange( FIntVector range );
	// �J�����g���W�ݒ�
	void		SetCurrentPosition( const FIntVector& pos );
	// �J�����g���W�擾
	FIntVector	GetCurrentPosition() const	{return m_CurrentWorldPos;}

	// �X�V
	virtual void	UpdateWorld();

	//! �^�[���i�s
	void	AdvanceTurn();
	//! �A�N�V�����`�F�C���ɒǉ�
	void	InsertActionChain( UNaTurnActionComponent* tac );

	// ���[�W�����擾
	UNaRegion*	GetRegion( const FIntVector& worldPos );
	// �`�����N�擾
	UNaChunk*	GetChunk( const FIntVector& chunkPos );
	// �`�����N�擾�i���[���h���W�j
	UNaChunk*	GetChunkFromWorld( FIntVector worldPos );

	//! �u���b�N�ݒ�
	virtual void	SetBlock( FIntVector pos, FNaWorldBlockWork& block );
	//! �u���b�N���擾
	bool			GetBlock( FIntVector worldPos, FNaWorldBlockWork& outVal );
	// �ڒn�Z������
	bool			FindGroundPos( FIntVector startPos, FIntVector& outPos );
	// �V��Z������
	bool			FindCeilPos( FIntVector startPos, FIntVector& outPos );

	//! ���[�W�����f�[�^�ǂݍ���
	virtual bool	ReadRegionData( FIntVector pos, TArray<uint8>& outVal ) override;
	//! ���[�W�����f�[�^��������
	virtual bool	WriteRegionData( FIntVector pos, const TArray<uint8>& inVal ) override;


	// �G���e�B�e�B����
	bool		FindEntity( FIntVector worldPos, TArray<UNaEntity*>& outVal );
	// �G���e�B�e�B���X�g�擾
	const TArray<UNaEntity*>&	GetEntities() const	{return m_SpawnEntities;}

	// ���[���h�X�e�[�W�ɃG���e�B�e�B�o�^
	bool		RegisterEntity( UNaEntity* entity );
	// ���[���h�X�e�[�W����G���e�B�e�B����
	void		UnregisterEntity( UNaEntity* entity );
	//! �G���e�B�e�BID���s
	uint32		IssueEntityID();

	// �G���e�B�e�B�X�|�[���i���̂̐����j
	bool		SpawnEntity( UNaEntity* entity, FIntVector pos );
	// �G���e�B�e�B�f�X�|�[���i���̂̍폜�j
	void		DespawnEntity( UNaEntity* entity );

	// ���[���h�ɔz�u
	bool		EnterEntity( UNaEntity* entity );
	// ���[���h���珜��
	void		LeaveEntity( UNaEntity* entity );

	// �A�N�V�����`�F�C���ɒǉ�
	void		AttachActionChain( UNaEntity* entity );
	// �A�N�V�����`�F�C�����珜��
	void		DetachActionChain( UNaEntity* entity );

	//! UE���[���h�擾
	UWorld*			GetWorldContext() const	{ return m_WorldContext; }
	//! ���[���h�A�N�^�[�擾
	ANaWorldActor*	GetWorldActor() const	{ return m_WorldActor; }

	//
	void		SetChunkLimit( FIntVector min, FIntVector max )	{m_ChunkMin = min; m_ChunkMax = max;}
	//! 
	FIntVector	GetChunkMin() const	{ return m_ChunkMin; }
	//! 
	FIntVector	GetChunkMax() const	{ return m_ChunkMax; }

	//! �}�b�v����
	void		CreateMap( FIntVector location, const UNaMapAsset* mapAsset );
	void		CreateMap( FIntVector location, const FStringAssetReference& mapAsset );
	//! 
	void		FindMap( FIntVector location, TArray<UNaMap*>& outArray );
	//! �}�b�v�擾
//	UNaMap*		GetMap( int32 idx ) const	{return m_MapData.IsValidIndex(idx) ? m_MapData[idx] : nullptr;}

	//! �w��`�����N�̃G���e�B�e�B���W
	void		GatherWorldEntities( const FIntVector& chunkPos, TArray<UNaEntity*>& outVal );

	void		SetCeilZ(int32 cz)	{m_CeilZ = cz;}
	int32		GetCeilZ() const	{return m_CeilZ;}

	void			SetWorldDirection(ENaDirection dir)	{m_WorldDirection = dir;}
	ENaDirection	GetWorldDirection() const			{return m_WorldDirection;}

	//! �C�x���g�}�l�[�W���擾
	virtual UNaEventManager*	GetEventManager()	{ return nullptr; }

protected:
	//
	FString		MakeWorldDirPath( int32 worldID ) const;
	//
	FString		GetWorldFilePath() const;
	//
	FString		GetRegionDirPath() const;
	//
	FString		GetMapDirPath() const;

public:
	
protected:
	//! �ŗLID
	FName		m_UID;
	//! �A�Z�b�gID
	FName		m_AssetID;
	//! �f�[�^ID
	uint32		m_DataID;

	//!
	FString		m_WorldPath;
	//! ���[���h�`�����N�͈͉���
	FIntVector	m_ChunkMin;
	//! ���[���h�`�����N�͈͏��
	FIntVector	m_ChunkMax;

	//! ���[���h�\����
	FText		m_DisplayName;

	//! ���[���h�W�F�l���[�^
	UPROPERTY(Transient)
	UNaWorldGenerator*	m_Generator;

	//! UE���[���h
	UPROPERTY(Transient)
	UWorld*				m_WorldContext;
	//! ���[���h�\���Ǘ��A�N�^�[
	UPROPERTY(Transient)
	ANaWorldActor*		m_WorldActor;
	//! �z�[���|�W�V����
	TArray<FIntVector>	m_HomePositions;

	// ����� //
	int32	m_Temperature;		// �C��
	int32	m_Humidity;			// ���x
	int32	m_AtmosPressure;	// �C��

	// �J�����g
	FIntVector			m_CurrentWorldPos;	// 
	FIntVector			m_CurrentChunkPos;	// 
	int32				m_CeilZ;			// 
	ENaDirection		m_WorldDirection;	// ���[���h�\�����p
	//! �A�N�e�B�u�`�����N�͈�
	FIntVector			m_ChunkRange;

	// ���C���n�`���
	UPROPERTY(Transient)
	TArray<UNaRegion*>				m_Regions;		// ���[�W��������
	TMap<FIntVector, UNaRegion*>	m_RegionMap;	// ���[�W�����A�N�Z�X�p�}�b�v
	TMap<FIntVector, UNaChunk*>		m_ChunkMap;		// �`�����N�A�N�Z�X�p�}�b�v�i���̂̓��[�W�������Ǘ��j

	// ���ԃf�[�^�}�b�v
	UPROPERTY()
	TArray<FNaMapEntry>				m_MapEntries;	// �}�b�v�G���g��
	UPROPERTY(Transient)
	TMap<int32, UNaMap*>			m_MapData;		// �W�J���̃}�b�v����

	// �G���e�B�e�B //
	UPROPERTY()
	TArray<UNaEntity*>				m_Entities;				// ���[���h�X�e�[�W�̃G���e�B�e�B���� //
	UPROPERTY()
	int32							m_NextEntityID;			// ���񃏁[���h�G���e�B�e�BID //
	UPROPERTY(Transient)
	TArray<UNaEntity*>				m_SpawnEntities;		// �X�|�[�����̃G���e�B�e�B�i�X�e�[�W���ʁj //
	

	// �^�[������ //
	UPROPERTY(Transient)
	TArray<UNaTurnActionComponent*>	m_ActionChain;		// �A�N�V�������X�g //
	UPROPERTY(Transient)
	UNaTurnActionComponent*			m_CurrentAction;	//  //
};
