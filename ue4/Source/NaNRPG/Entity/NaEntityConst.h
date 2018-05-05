// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc/NaEnums.h"

#include "Assets/Entity/NaEntityDataTable.h"
#include "Assets/Entity/NaActorDataTable.h"

#include "NaEntityConst.generated.h"

// エンティティ内部データ
USTRUCT()
struct FNaEntityProfile
{
	GENERATED_BODY()

	// ソースアセットID //
	UPROPERTY()
	FName	AssetID;

	// 表示名（このエンティティデータの名前。「タンス」とか「街の貴族」とか） //
	UPROPERTY()
	FString	DisplayName;
	// 使用アクター //
	UPROPERTY()
	FName	ActorName;
	// ユニーク表示名（生成されるキャラクター・オブジェクトの固有名。空白はルールに従ってランダム生成） //
	UPROPERTY()
	FString	UniqueName;

	//!
	UPROPERTY()
	int32	AIType;

	//! アセットから基本情報を構築
	void	CopyFromAsset( const FNaEntityDataAsset& asset )
	{
		AssetID	= asset.Name;

		DisplayName	= asset.DisplayName.ToString();
		ActorName	= asset.ActorName;
		UniqueName	= asset.UniqueName.ToString();

		AIType	= asset.AIType;
	}
};

//! シリアライズ：FName
void SerializeFName( FArchive &ar, FName& obj );
//! シリアライズ：FNaEntityProfile
FArchive& operator << ( FArchive &ar, FNaEntityProfile& obj );

// アバターデータ
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

	// 身体系パラメータ
	float	Str;
	float	Vit;
	float	Agi;
	float	Dec;
	float	End;

	// 精神系パラメータ
	float	Mag;
	float	Int;
	float	Men;
	float	Wil;
	float	Cha;
};

//! キャラクターステータス
USTRUCT()
struct FNaCharacterStatus
{
	GENERATED_BODY()

	// 
	int16	Level;		//! レベル
	int16	LevelMax;	//! 最大到達レベル

	int32	HP;			// Health
	int32	HPMax;		// 
	int32	SP;			// Stamina
	int32	SPMax;		// 
	int32	MP;			// Mana
	int32	MPMax;		// 

	//
	int32	Speed;		//! 行動速度

	// メインパラメータ
	FNaBaseParameter	BaseParam;		//! 基礎値

	// ポテンシャル
	FNaBaseParameter	Potencial;		//! ポテンシャル現在値
	FNaBaseParameter	PotencialMax;	//! ポテンシャル最大値
	FNaBaseParameter	PotencialExp;	//! ポテンシャル経験値

};

//! スキルデータ
USTRUCT()
struct FNaCharacterSkill
{
	GENERATED_BODY()

	//
	FName	SkillID;		//! スキルID
	int32	SkillLv;		//! スキルレベル
	float	Exp;			//! スキル経験値
};

//! 才能データ
USTRUCT()
struct FNaCharacterTalent
{
	GENERATED_BODY()

	//
	FName	TalentID;		//! 才能ID
	int16	TalentLv;		//! 才能レベル
};

//! 技データ
USTRUCT()
struct FNaCharacterArts
{
	GENERATED_BODY()

	//
	FName	ID;			//! アーツID
	int16	Lv;			//! アーツレベル
};

//! スペルデータ
USTRUCT()
struct FNaCharacterSpell
{
	GENERATED_BODY()

	//
	int16	Lv;			//! スペルレベル
	int16	Rank;		//! スペルランク

	//
//	TArray<FSpellEffect>	Effects;
};

//! 装備情報
USTRUCT()
struct FNaCharacterEquipPart
{
	GENERATED_BODY()

	//! 部位タイプ
	ENaItemEquipPart::Type	Part;
	//! 装備品
	class UNaItem*			Item;
	//! 装備品（サブ）
	class UNaItem*			SubItem;
};
