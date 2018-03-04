// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc/NaEnums.h"

#include "Assets/Entity/NaEntityDataTable.h"
#include "Assets/Entity/NaActorDataTable.h"

#include "NaEntityConst.generated.h"

// �G���e�B�e�B�����f�[�^
USTRUCT()
struct FNaEntityProfile
{
	GENERATED_BODY()

	// �\�[�X�A�Z�b�gID //
	UPROPERTY()
	FName	AssetID;

	// �\�����i���̃G���e�B�e�B�f�[�^�̖��O�B�u�^���X�v�Ƃ��u�X�̋M���v�Ƃ��j //
	UPROPERTY()
	FString	DisplayName;
	// �g�p�A�N�^�[ //
	UPROPERTY()
	FName	ActorName;
	// ���j�[�N�\�����i���������L�����N�^�[�E�I�u�W�F�N�g�̌ŗL���B�󔒂̓��[���ɏ]���ă����_�������j //
	UPROPERTY()
	FString	UniqueName;

	//!
	UPROPERTY()
	int32	AIType;

	//! �A�Z�b�g�����{�����\�z
	void	CopyFromAsset( const FNaEntityDataAsset& asset )
	{
		AssetID	= asset.Name;

		DisplayName	= asset.DisplayName.ToString();
		ActorName	= asset.ActorName;
		UniqueName	= asset.UniqueName.ToString();

		AIType	= asset.AIType;
	}
};

//! �V���A���C�Y�FFName
void SerializeFName( FArchive &ar, FName& obj );
//! �V���A���C�Y�FFNaEntityProfile
FArchive& operator << ( FArchive &ar, FNaEntityProfile& obj );

// �A�o�^�[�f�[�^
USTRUCT()
struct FNaEntityAvatar
{
	GENERATED_BODY()

};

//! 
USTRUCT()
struct FNaBaseParameter
{
	GENERATED_BODY()

	// �g�̌n�p�����[�^
	float	Str;
	float	Vit;
	float	Agi;
	float	Dec;
	float	End;

	// ���_�n�p�����[�^
	float	Mag;
	float	Int;
	float	Men;
	float	Wil;
	float	Cha;
};

//! �L�����N�^�[�X�e�[�^�X
USTRUCT()
struct FNaCharacterStatus
{
	GENERATED_BODY()

	// 
	int16	Level;		//! ���x��
	int16	LevelMax;	//! �ő哞�B���x��

	int32	HP;			// Health
	int32	HPMax;		// 
	int32	SP;			// Stamina
	int32	SPMax;		// 
	int32	MP;			// Mana
	int32	MPMax;		// 

	//
	int32	Speed;		//! �s�����x

	// ���C���p�����[�^
	FNaBaseParameter	BaseParam;		//! ��b�l

	// �|�e���V����
	FNaBaseParameter	Potencial;		//! �|�e���V�������ݒl
	FNaBaseParameter	PotencialMax;	//! �|�e���V�����ő�l
	FNaBaseParameter	PotencialExp;	//! �|�e���V�����o���l

};

//! �X�L���f�[�^
USTRUCT()
struct FNaCharacterSkill
{
	GENERATED_BODY()

	//
	FName	SkillID;		//! �X�L��ID
	int32	SkillLv;		//! �X�L�����x��
	float	Exp;			//! �X�L���o���l
};

//! �˔\�f�[�^
USTRUCT()
struct FNaCharacterTalent
{
	GENERATED_BODY()

	//
	FName	TalentID;		//! �˔\ID
	int16	TalentLv;		//! �˔\���x��
};

//! �Z�f�[�^
USTRUCT()
struct FNaCharacterArts
{
	GENERATED_BODY()

	//
	FName	ID;			//! �A�[�cID
	int16	Lv;			//! �A�[�c���x��
};

//! �X�y���f�[�^
USTRUCT()
struct FNaCharacterSpell
{
	GENERATED_BODY()

	//
	int16	Lv;			//! �X�y�����x��
	int16	Rank;		//! �X�y�������N

	//
//	TArray<FSpellEffect>	Effects;
};

//! �������
USTRUCT()
struct FNaCharacterEquipPart
{
	GENERATED_BODY()

	//! ���ʃ^�C�v
	ENaItemEquipPart::Type	Part;
	//! �����i
	class UNaItem*			Item;
	//! �����i�i�T�u�j
	class UNaItem*			SubItem;
};
