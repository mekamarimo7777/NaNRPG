// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "Misc/NaStructs.h"

#include "NaItemDataTable.generated.h"

USTRUCT()
struct FNaItemProperty
{
	GENERATED_BODY()

	// 付与種別
	UPROPERTY(EditDefaultsOnly, Category = General)
	TEnumAsByte<ENaItemPropertyFactor::Type>	Factor;
	// 付与強度
	UPROPERTY(EditDefaultsOnly, Category = General)
	uint8	Intensity;
	// 持続時間（ターン）
	UPROPERTY(EditDefaultsOnly, Category = General)
	int16	Duration;

	// 発動条件
	UPROPERTY(EditDefaultsOnly, Category = Action, Meta = (Bitmask, BitmaskEnum = "ENaActionTrigger"))
	int32	Action;
	// 効果種別
	UPROPERTY(EditDefaultsOnly, Category = Action)
	TEnumAsByte<ENaActionEffect::Type>	Effect;
	// パラメータ
	UPROPERTY(EditDefaultsOnly, Category = Action)
	int32	Params[4];
};

USTRUCT()
struct FNaItemDataAsset : public FTableRowBase
{
	GENERATED_BODY()

	// 固有ID //
	UPROPERTY(EditDefaultsOnly, Category = General)
	int32	UID;
	// 内部識別名 //
	UPROPERTY(EditDefaultsOnly, Category = General)
	FName	Name;

	// 表示名 //
	UPROPERTY(EditDefaultsOnly, Category = General)
	FText	DisplayName;
	// 表示名２（バリエーション） //
//	UPROPERTY(EditDefaultsOnly, Category = General)
//	FText	DisplayName;
	// 不確定名 //
	UPROPERTY(EditDefaultsOnly, Category = General)
	FText	UncertainName;
	// 解説 //
	UPROPERTY(EditDefaultsOnly, Category = General, Meta = (Multiline = true))
	FText	FlavorText;

	// 表示用エンティティ名（未設定の場合は"Item"固定） //
	UPROPERTY(EditDefaultsOnly, Category = Render)
	FName	EntityName;
	// 表示用追加パラメータ //
	UPROPERTY(EditDefaultsOnly, Category = Render)
	int32	EntityParam;
	// アイコンID（256ごとに別のテクスチャから表示） //
	UPROPERTY(EditDefaultsOnly, Category = Render)
	int16	IconID;
	// 色相（仮）
	UPROPERTY(EditDefaultsOnly, Category = Render)
	int16	IconHue;
	// 彩度（仮）
	UPROPERTY(EditDefaultsOnly, Category = Render)
	int8	IconSaturation;

	// アイテムランク //
	UPROPERTY(EditDefaultsOnly, Category = Param)
	int8	Rank;
	// レアリティ //
	UPROPERTY(EditDefaultsOnly, Category = Param)
	int8	Rarity;
	// 最大スタック //
	UPROPERTY(EditDefaultsOnly, Category = Param)
	int16	MaxStackCount;
	// 基本重量 //
	UPROPERTY(EditDefaultsOnly, Category = Param)
	int16	Weight;

	//! 装備可能部位
	UPROPERTY(EditDefaultsOnly, Category = Equipment, Meta = (Bitmask, BitmaskEnum = "ENaItemEquipPart"))
	int32	EquipFlag;

	//! 性質
	UPROPERTY(EditDefaultsOnly, Category = Property)
	TArray<FNaItemProperty>	Properties;

	FNaItemDataAsset()
	: MaxStackCount( 1 )
	{
	}
};

//! シリアライズ：FNaItemProperty
FArchive& operator << ( FArchive &ar, FNaItemProperty& obj );
