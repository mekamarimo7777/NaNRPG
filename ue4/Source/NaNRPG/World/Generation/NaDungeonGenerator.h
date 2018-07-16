// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Assets/World/NaMapAsset.h"

#include "NaDungeonGenerator.generated.h"

class UNaChunk;

/**
 * ダンジョンェネレータ
 */
UCLASS()
class NANRPG_API UNaDungeonGenerator : public UObject
{
	GENERATED_BODY()

public:
	//! 部屋
	struct FRoom
	{
		//! ID
		int32		ID;
		//! 座標
		FIntPoint	Pos;
		//! サイズ
		FIntPoint	Size;
		//! 中心座標
		FIntPoint	Center;

		//! メインの部屋
		bool		IsMain;
		//! サブの部屋
		bool		IsSub;

		//! 接続情報
		TArray<int32>	Connection;

		//! ワーク座標
		FVector2D	WorkPos;
	};

	//! 通路
	struct FCorridor
	{
		//! ID
		int32		ID;
		//! 座標
		FIntPoint	Pos;
		//! サイズ
		FIntPoint	Size;
	};
	
public:
	//! コンストラクタ
	UNaDungeonGenerator();

	//! 生成
	void	Generate( const UNaMapAsset* asset );

	//! 部屋リスト
	const TArray<FRoom>&		GetRooms() const			{ return m_Rooms; }
	//! 通路リスト
	const TArray<FCorridor>&	GetCorridors() const		{ return m_Corridors; }
	//! 
	const TMap<FIntVector, UNaChunk*>	GetChunks() const	{ return m_Chunks; }

protected:
	//! 正規分布
	FIntPoint	GetRandomPointInCircle( float radius );
	//! 部屋分散
	void		DiscreteRooms( TArray<FRoom>& rooms, int32 maxLoop = 1000 );
	//! 通路生成
	void		CreateCorridors();
	//! サブ部屋追加
	void		IncludeSubRooms();

	//! チャンクデータ構築
	void		BuildChunkData();
	//! 
	void		SetBlock( FIntVector pos, FNaWorldBlockWork& block );
	//! チャンク生成or取得
	UNaChunk*	GetChunk( FIntVector cpos );

protected:
	//! 部屋
	TArray<FRoom>		m_Rooms;
	//! 通路
	TArray<FCorridor>	m_Corridors;

	//! チャンクデータ（マイナス座標は無い）
	UPROPERTY()
	TMap<FIntVector, UNaChunk*>	m_Chunks;
};
