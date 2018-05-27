// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "NaMapPartsAsset.h"

#include "World/NaWorldDefinition.h"

#include "NaMapAsset.generated.h"

//! マップ固有チャンク情報
USTRUCT()
struct FNaChunkData
{
	GENERATED_BODY()

	static const int	DIM_X = 16;
	static const int	DIM_Y = 16;
	static const int	DIM_Z = 16;
	static const int	SHIFT_Y = 4;
	static const int	SHIFT_Z = 8;
	static const int	ELEMENTS = DIM_X * DIM_Y * DIM_Z;

	//! チャンク座標
	UPROPERTY()
	FIntVector	Position;
	// ブロックID
	UPROPERTY()
	uint8		ID[ELEMENTS];
	// ブロックパラメータ
	UPROPERTY()
	uint8		Param[ELEMENTS];
	// 汎用データ
	UPROPERTY()
	uint32		Meta[ELEMENTS];
};

//! パーツエントリ情報
USTRUCT()
struct FNaMapPartsEntry
{
	GENERATED_BODY()

	//! パーツアセットID
	UPROPERTY()
	FName		ID;
	//! 配置座標
	UPROPERTY()
	FIntVector	Position;
	//! 配置サイズ
	UPROPERTY()
	FIntVector	Size;
	//! 向き
	UPROPERTY()
	int32		Direction;
};

// ランダムマップ生成処理
USTRUCT()
struct FNaMapProcess
{
	GENERATED_BODY()

	//! 種別
	UPROPERTY(EditAnywhere, Category = "General")
	int32		Type;
	UPROPERTY(EditAnywhere, Category = "General")
	//! 広さ（ブロック単位）
	FVector2D	Range;
};

struct FNaMapGen;
struct FNaMapNode;

/**
 * マップデータアセット
 */
UCLASS()
class NANRPG_API UNaMapAsset : public UObject
{
	GENERATED_BODY()
	
public:
	// 簡易ランダムマップ生成 //
	void	CreateSimpleRandomMap( FIntVector size );
	//
	void	DivideNode( FNaMapGen* gen );
	//
	void	CreateRoom( FNaMapGen* gen );
	//
	void	CreateCorridor( FNaMapGen* gen );
	//
	void	ConnectRoom( FNaMapNode* src, FNaMapNode* dst );

	//! 
	void	ResizeMap( const FIntVector& size );
	//! 
	void	SetBlock( const FIntVector& pos, const FNaWorldBlockWork& work );

	//! 
	TArray<FNaChunkData>&	GetChunks()	{ return m_Chunks; }

protected:
	

public:
	//! ID
	UPROPERTY()
	int32		m_ID;
	//! マップサイズ（チャンク単位）
	UPROPERTY()
	FIntVector	m_MapSize;

	//! ランダム生成設定

	//! 固定チャンク
	UPROPERTY()
	TArray<FNaChunkData>		m_Chunks;
	//! 固定配置構造物
	UPROPERTY()
	TArray<FNaMapPartsEntry>	m_Parts;

	//! エンティティリスト
	UPROPERTY(EditDefaultsOnly, Category = "Entity")
	TArray<FNaEntityEntry>		m_Entities;

	//! 一時情報

};
