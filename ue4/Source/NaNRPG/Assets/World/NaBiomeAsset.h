// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Assets/NaDataAsset.h"

#include "NaWorldAssetConst.h"

#include "NaBiomeAsset.generated.h"


//! バイオーム種別
UENUM()
enum class ENaBiomeType
{

	ENUM_MAX	UMETA(Hidden)
};

/**
 * バイオーム定義アセット
 */
UCLASS()
class NANRPG_API UNaBiomeAsset : public UNaDataAsset
{
	GENERATED_BODY()

public:
	//! 表示名
	UPROPERTY( EditDefaultsOnly, Category = "General" )
	FText			DisplayName;
	//! バイオームタイプ
	UPROPERTY( EditDefaultsOnly, Category = "General" )
	ENaBiomeType	Type;
	
	//! 
	UPROPERTY( EditDefaultsOnly, Category = "Block" )
	FName			a;

};
