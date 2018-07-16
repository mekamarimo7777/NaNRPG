// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Assets/World/NaWorldAsset.h"

#include "NaWorldGenerator.generated.h"

class UNaWorld;
class UNaRegion;
class UNaChunk;

/**
 * ワールドジェネレータ
 */
UCLASS()
class NANRPG_API UNaWorldGenerator : public UObject
{
	GENERATED_BODY()

public:
	
public:
	//! コンストラクタ
	UNaWorldGenerator();

	//! アセット設定
	void	SetWorldAsset( UNaWorldAsset* asset );
	//! チャンク生成
	void	GenerateChunk( UNaChunk* chunk );

protected:
	//! 基本地形生成
	void	MakeTerrain( UNaChunk* chunk );
	//! 平坦な地形生成
	void	MakeFlatland( UNaChunk* chunk );

	//! ハイトマップ生成
	void	GenerateHeightMap();
	//! バイオーム生成
	void	GenerateBiome();

	//! マップデータ書き込み
	void	CopyMapData( UNaChunk* chunk );

public:
	
protected:
	//! ワールド
	UNaWorld*		m_World;
	//! 生成対象チャンク
	UNaChunk*		m_Chunk;
	//! ワールドアセット
	UNaWorldAsset*	m_Asset;

	//! ハイトマップ
	TArray<int32>	m_HeightMap;
	//! バイオーム
	TArray<int32>	m_Biome;
};
