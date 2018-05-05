// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaDatabaseConst.generated.h"

/**
 * ワールド登録情報
 */
USTRUCT()
struct NANRPG_API FNaWorldRecord
{
	GENERATED_BODY()

	//! 固有ID
	UPROPERTY()
	FName	UID;
	//! データID（保存フォルダの識別用。ランダムに生成）
	UPROPERTY()
	uint32	DataID;
};
