// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "Misc/NaStructs.h"

#include "NaSkillDataTable.generated.h"

//! スキルデータ
USTRUCT()
struct FNaSkillDataAsset : public FTableRowBase
{
	GENERATED_BODY()

	//! 固有ID
	UPROPERTY(EditDefaultsOnly, Category = General)
	int32	UID;
	//! 内部識別名
	UPROPERTY(EditDefaultsOnly, Category = General)
	FName	Name;

	//! 表示名
	UPROPERTY(EditDefaultsOnly, Category = General)
	FText	DisplayName;

};
