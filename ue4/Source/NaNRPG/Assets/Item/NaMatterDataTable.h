// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "Misc/NaStructs.h"

#include "NaMatterDataTable.generated.h"

USTRUCT()
struct FNaMatterProperty
{
	GENERATED_BODY()

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

//! アイテム素材データ（Materialでは紛らわしいのでMatter）
USTRUCT()
struct FNaMatterDataAsset : public FTableRowBase
{
	GENERATED_BODY()

	// 固有ID //
	UPROPERTY(EditDefaultsOnly, Category = General)
	int32	UID;
	// 内部識別名 //
	UPROPERTY(EditDefaultsOnly, Category = General)
	FName	Name;

	//! 素材名
	UPROPERTY(EditDefaultsOnly, Category = General)
	FText	DisplayName;

	//! 素材デフォルトカラー
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	FColor	Color;

	//! 素材性質
	UPROPERTY(EditDefaultsOnly, Category = Property)
	TArray<FNaMatterProperty>	Properties;
};
