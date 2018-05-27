// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "World/NaChunk.h"

#include "Assets/Map/NaMapAsset.h"

#include "NaMap.generated.h"

/**
 * 中間マップインスタンス（構造物データから生成済みのチャンクで構成される）
 */
UCLASS()
class NANRPG_API UNaMap : public UObject
{
	GENERATED_BODY()
	
public:
	//! マップ生成
	void	Create( UNaWorld* naw, int32 uid, FIntVector pos, const UNaMapAsset* asset );

	//! マップ保存
	void	SaveMap( FString path );
	//! マップ読み込み
	void	LoadMap( FString path, int32 id );

	//!	チャンク取得
	UNaChunk*	GetChunk( FIntVector cpos );
	//! マップクローズ
	void		CloseMap();

	//! 
	virtual void	Serialize( FArchive& ar ) override;

	//! @deprecated マップアセットからインスタンス生成
	void	Instantiate( UNaMapAsset* src );
	//
	void	CopyPartsToChunk( FNaMapPartsEntry* parts, UNaChunk* chunk );
	
	//! 
	void		SetLocation(FIntVector pos)	{ m_MapLocation = pos; }
	//! 
	FIntVector	GetLocation() const			{ return m_MapLocation; }

protected:
	//! マップID
	int32		m_MapID;
	//! マップアセット
	int32		m_AssetID;
	//! マップ配置座標
	FIntVector	m_MapLocation;
	//! マップサイズ（チャンク単位）
	FIntVector	m_MapSize;

	// チャンク情報 //
	UPROPERTY()
	TArray<UNaChunk*>				m_Chunks;		// 実体保持用 //
	TMap<FIntVector, UNaChunk*>		m_ChunkMap;
	// チャンクキャッシュ //
	TMap<FIntVector, TArray<uint8>>	m_ChunkCache;
};
