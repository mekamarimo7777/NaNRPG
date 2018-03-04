// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "Misc/NaStructs.h"

#include "NaDataAsset.generated.h"

/**
 * NaNRPG�f�[�^�A�Z�b�g���
 */
UCLASS()
class NANRPG_API UNaDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

public:
	//! ���j�[�NID
	UPROPERTY( EditDefaultsOnly, AssetRegistrySearchable, Category = General )
	FName	UID;

};
