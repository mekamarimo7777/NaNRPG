// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc/NaEnums.h"

#include "NaStructs.generated.h"

// キャラアクター定義 //
USTRUCT()
struct FNaActorDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Actor")
	FName	UID;

	UPROPERTY(EditDefaultsOnly, Category="Actor")
	USkeletalMesh*	SkeletalMesh;
	UPROPERTY(EditDefaultsOnly, Category="Actor")
	UStaticMesh*	StaticMesh;

	UPROPERTY(EditDefaultsOnly, Category="Actor")
	TSubclassOf<UAnimInstance>	AnimInstance;

	UPROPERTY(EditDefaultsOnly, Category="Actor")
	UAnimMontage*	Animation[ENaActorAnimation::ENUM_MAX];
};

//! キー数値ペア
USTRUCT()
struct FNaKeyValuePair
{
	GENERATED_BODY()

	//! キー
	UPROPERTY()
	FName	Key;
	//! 値
	UPROPERTY()
	FString	Value;
};

// ダイス数値 //
USTRUCT()
struct FNaDiceValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Common")
	int16	DiceFaces;
	UPROPERTY(EditAnywhere, Category = "Common")
	int8	DiceCount;
	UPROPERTY(EditAnywhere, Category = "Common")
	int8	Addition;
};
