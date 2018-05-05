// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaDatabaseConst.h"

#include "Entity/Character/NaEntityPlayer.h"

#include "NaGameDatabase.generated.h"

/**
 * �O���[�o���Q�[���f�[�^�x�[�X�i�ŏ�ʂ̃Z�[�u�f�[�^�j
 */
UCLASS()
class NANRPG_API UNaGameDatabase : public UObject
{
	GENERATED_BODY()
	
public:
	//
	static UNaGameDatabase*	GetDB();
	//
	static FString	GameSavedDir();

	//
	bool	LoadDB(FString name);
	//
	bool	SaveDB();

	//
	virtual	void	Serialize( FArchive& ar ) override;

	//! �v���C���[�G���e�B�e�B�o�^
	void				RegisterPlayer( UNaEntityPlayer* player );
	//! �O���[�o���G���e�B�e�B�o�^
	void				RegisterGlobalEntity( UNaEntity* entity );
	//! �w�胏�[���h���G���e�B�e�B���W
	void				GatherEntities( FName id, TArray<UNaEntity*>& outVal );
	//! �v���C���[�G���e�B�e�B�擾
	UNaEntityPlayer*	GetPlayer() const	{ return m_Player; }

	//! ���[���h�G���g���o�^
	void				RegisterWorldEntry( FName id, uint32 dataID );
	//! ���[���h�G���g���擾
	FNaWorldRecord*		FindWorldEntry( FName id );
	//! ���[���h�G���g���m�F
	bool				ExistWorldEntry( FName id );
	//! ���[���h�f�[�^ID����
	uint32				GenerateWorldDataID();

protected:
	//
	FString	GetDBDirName();
	//
	FString	GetDBFileName();

public:
	FString	m_DataName;

	//! �v���C���[����
	UPROPERTY()
	UNaEntityPlayer*			m_Player;
	//! �p�[�e�B�����o�[
	UPROPERTY()
	TArray<UNaEntityCharacter*>	m_PartyMember;
	//! �O���[�o���G���e�B�e�B
	UPROPERTY()
	TArray<UNaEntity*>			m_GlobalEntities;

	//! �������[���h�ꗗ
	UPROPERTY()
	TArray<FNaWorldRecord>	m_WorldRecords;
	//! �J�����g�̃��[�J���}�b�v�i���ݒ莞�̓O���[�o���}�b�v�j
	UPROPERTY()
	FName					m_WorldID;

	//! �C�x���g�t���O
//	UPROPERTY()
//	TBitArray<FDefaultBitArrayAllocator>	m_EventFlags;
	//! �C�x���g�ϐ�
	UPROPERTY()
	TMap<FName, FString>					m_EventVariables;

protected:



};
