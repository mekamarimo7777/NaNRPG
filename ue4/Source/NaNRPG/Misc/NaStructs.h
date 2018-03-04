// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc/NaEnums.h"

#include "NaStructs.generated.h"

// �L�����A�N�^�[��` //
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

//! �L�[���l�y�A
USTRUCT()
struct FNaKeyValuePair
{
	GENERATED_BODY()

	//! �L�[
	UPROPERTY()
	FName	Key;
	//! �l
	UPROPERTY()
	FString	Value;
};

// �_�C�X���l //
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
