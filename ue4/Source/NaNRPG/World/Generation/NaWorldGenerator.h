// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Assets/Map/NaWorldAsset.h"

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

	//! チャンク生成
	void	GenerateChunk( UNaChunk* chunk );

protected:
	//! 基本地形生成
	void	MakeTerrain( UNaChunk* chunk );
	//! 平坦な地形生成
	void	MakeFlatland( UNaChunk* chunk );

	//! マップデータ書き込み
	void	CopyMapData( UNaChunk* chunk );

public:
	
protected:
	//! 
	UNaWorld*	m_World;

};
