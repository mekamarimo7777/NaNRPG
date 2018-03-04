// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	//! �v���C���[�G���e�B�e�B�擾
	UNaEntityPlayer*	GetPlayer() const	{ return m_Player; }

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
	//! ���̑��ėp�O���[�o���G���e�B�e�B
	UPROPERTY()
	TArray<UNaEntity*>			m_GlobalEntities;

	//! �J�����g�̃��[�J���}�b�v
	UPROPERTY()
	FName	m_CurrentWorld;
	//! �񃆃j�[�N�}�b�v�̕����o��ID
	UPROPERTY()
	int32	m_NextWorldID;

	//! �C�x���g�t���O
//	UPROPERTY()
//	TBitArray<FDefaultBitArrayAllocator>	m_EventFlags;
	//! �C�x���g�ϐ�
	UPROPERTY()
	TMap<FName, FString>					m_EventVariables;

protected:



};
