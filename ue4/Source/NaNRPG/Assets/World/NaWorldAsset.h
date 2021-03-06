﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Assets/NaDataAsset.h"

#include "NaWorldAssetConst.h"
#include "NaMapAsset.h"

#include "NaWorldAsset.generated.h"

//! ワールド配置マップエントリ
USTRUCT()
struct FNaWorldMapEntry
{
	GENERATED_BODY()

	//! マップアセット
	UPROPERTY( EditDefaultsOnly, Category = "General", Meta = (AllowedClasses = "NaMapAsset") )
	FStringAssetReference	Map;
	//! 配置座標（チャンク単位）
	UPROPERTY( EditDefaultsOnly, Category = "General" )
	FIntVector				Location;
};

/**
 * ワールド定義アセット
 */
UCLASS()
class NANRPG_API UNaWorldAsset : public UNaDataAsset
{
	GENERATED_BODY()
	
public:
	//! 表示名
	UPROPERTY( EditDefaultsOnly, Category = "General" )
	FText		DisplayName;
	//! ワールド下限チャンク
	UPROPERTY( EditDefaultsOnly, Category = "General" )
	FIntPoint	WorldMin;
	//! ワールド上限チャンク
	UPROPERTY( EditDefaultsOnly, Category = "General" )
	FIntPoint	WorldMax;

	//! シード値（0でランダム）
	UPROPERTY( EditDefaultsOnly, Category = "World" )
	int32				WorldSeed;
	//! 基本生成モード
	UPROPERTY( EditDefaultsOnly, Category = "World" )
	ENaTerrainMode		WorldTerrain;
	//! 気候タイプ
	UPROPERTY( EditDefaultsOnly, Category = "World" )
	ENaBiomeMode		WorldBiome;
	//! 標高基準高さ
	UPROPERTY( EditDefaultsOnly, Category = "World" )
	int32				WorldBaseHeight;

	//! 配置マップアセット
	UPROPERTY( EditDefaultsOnly, Category = "Map" )
	TArray<FNaWorldMapEntry>	MapEntries;

	//! 初期エンティティ
	UPROPERTY( EditDefaultsOnly, Category = "Entity" )
	TArray<FNaEntityEntry>		Entities;
};
