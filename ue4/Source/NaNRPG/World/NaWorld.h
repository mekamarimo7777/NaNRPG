// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Misc/NaEnums.h"

#include "NaRegion.h"
#include "Map/NaMap.h"
#include "Generation/NaWorldGenerator.h"

#include "World/NaWorldDefinition.h"

#include "NaWorld.generated.h"

class ANaWorldActor;
class UNaWorldAsset;
class UNaWorldManager;
class UNaEntity;
class UNaEntityPlayer;
class UNaTurnActionComponent;
class UNaEventManager;

USTRUCT()
struct FNaMapEntry
{
	GENERATED_BODY()

	//! 固有ID
	UPROPERTY()
	int32		MapID;
	//! 配置チャンク座標
	UPROPERTY()
	FIntVector	Location;
	//! 配置範囲
	UPROPERTY()
	FIntVector	Range;
};

//! 
class INaWorldDataAccessor
{
	//! リージョンデータ読み込み
	virtual bool	ReadRegionData( FIntVector pos, TArray<uint8>& outVal )			{ return false; }
	//! リージョンデータ書き込み
	virtual bool	WriteRegionData( FIntVector pos, const TArray<uint8>& inVal )	{ return false; }
};

/**
 * ワールドインスタンス
 */
UCLASS()
class NANRPG_API UNaWorld : public UObject, public INaWorldDataAccessor
{
	GENERATED_BODY()

public:
	// コンストラクタ
	UNaWorld();

	//! 使用開始準備
	virtual void	Setup( ANaWorldActor* actor );
	//! 更新
	virtual void	Update( float DeltaTime );

	//! 新規作成
	virtual bool	CreateWorld( FName uid, FName assetID );
	//! 開く
	bool			OpenWorld( int32 dataID );
	//! 閉じる
	void			CloseWorld( bool isSave = true );

	//! 固有ID取得
	FName	GetUID() const		{ return m_UID; }
	//! アセットID取得
	FName	GetAssetID() const	{ return m_AssetID; }
	//! データID取得
	uint32	GetDataID() const	{ return m_DataID; }
	//! クローズ判定
	bool	IsClosed() const	{ return m_WM == nullptr; }

	//! シリアライズ
	virtual void	Serialize( FArchive& ar ) override;

	//! 
	void		SetChunkRange( FIntVector range );
	// カレント座標設定
	void		SetViewOrigin( const FIntVector& pos );
	// カレント座標取得
	FIntVector	GetViewOrigin() const	{ return m_ViewOrigin; }

	//! 内部情報再評価
	virtual void	Evaluate();

	// リージョン取得
	UNaRegion*	GetRegion( const FIntVector& worldPos );
	// チャンク取得
	UNaChunk*	GetChunk( const FIntVector& chunkPos );
	// チャンク取得（ワールド座標）
	UNaChunk*	GetChunkFromWorld( FIntVector worldPos );

	//! ブロック設定
	virtual void	SetBlock( FIntVector pos, FNaWorldBlockWork& block );
	//! ブロック情報取得
	bool			GetBlock( FIntVector worldPos, FNaWorldBlockWork& outVal );
	// 接地セル検索
	bool			FindGroundPos( FIntVector startPos, FIntVector& outPos );
	// 天井セル検索
	bool			FindCeilPos( FIntVector startPos, FIntVector& outPos );

	//! リージョンデータ読み込み
	virtual bool	ReadRegionData( FIntVector pos, TArray<uint8>& outVal ) override;
	//! リージョンデータ書き込み
	virtual bool	WriteRegionData( FIntVector pos, const TArray<uint8>& inVal ) override;


	// エンティティ検索
	bool		FindEntity( FIntVector worldPos, TArray<UNaEntity*>& outVal );
	// エンティティリスト取得
	const TArray<UNaEntity*>&	GetEntities() const	{return m_SpawnEntities;}

	// ワールドステージにエンティティ登録
	bool		RegisterEntity( UNaEntity* entity );
	// ワールドステージからエンティティ除去
	void		UnregisterEntity( UNaEntity* entity );
	//! エンティティID発行
	uint32		IssueEntityID();
	//! 無効エンティティの削除
	void		SweepEntities();

	// エンティティスポーン（実体の生成）
	bool		SpawnEntity( UNaEntity* entity, FIntVector pos );
	// エンティティデスポーン（実体の削除）
	void		DespawnEntity( UNaEntity* entity );

	// ワールドに配置
	bool		EnterEntity( UNaEntity* entity );
	// ワールドから除去
	void		LeaveEntity( UNaEntity* entity );

	//! ワールドアクター取得
	ANaWorldActor*		GetWorldActor() const	{ return m_WorldActor; }
	//! ワールドマネージャ取得
	UNaWorldManager*	GetWM() const			{ return m_WM; }
	//! ワールドジェネレータ取得
	UNaWorldGenerator*	GetGenerator() const	{ return m_Generator; }
	//! UEワールド取得
	UWorld*				GetWorldContext() const;
	
	//
	void		SetChunkLimit( FIntVector min, FIntVector max )	{m_ChunkMin = min; m_ChunkMax = max;}
	//! 
	FIntVector	GetChunkMin() const	{ return m_ChunkMin; }
	//! 
	FIntVector	GetChunkMax() const	{ return m_ChunkMax; }

	//! マップ生成
	void		CreateMap( FIntVector location, const UNaMapAsset* mapAsset );
	void		CreateMap( FIntVector location, const FStringAssetReference& mapAsset );
	//! 
	void		FindMap( FIntVector location, TArray<UNaMap*>& outArray );
	//! マップ取得
//	UNaMap*		GetMap( int32 idx ) const	{return m_MapData.IsValidIndex(idx) ? m_MapData[idx] : nullptr;}

	//! 指定チャンクのエンティティ収集
	void		GatherWorldEntities( const FIntVector& chunkPos, TArray<UNaEntity*>& outVal );

	void		SetCeilZ(int32 cz)	{m_CeilZ = cz;}
	int32		GetCeilZ() const	{return m_CeilZ;}

	void			SetWorldDirection(ENaDirection dir)	{m_WorldDirection = dir;}
	ENaDirection	GetWorldDirection() const			{return m_WorldDirection;}

protected:
	//
	FString		MakeWorldDirPath( int32 worldID ) const;
	//
	FString		GetWorldFilePath() const;
	//
	FString		GetRegionDirPath() const;
	//
	FString		GetMapDirPath() const;

protected:
	//! 固有ID
	FName		m_UID;
	//! アセットID
	FName		m_AssetID;
	//! データID
	uint32		m_DataID;

	//!
	FString		m_WorldPath;
	//! ワールドチャンク範囲下限
	FIntVector	m_ChunkMin;
	//! ワールドチャンク範囲上限
	FIntVector	m_ChunkMax;

	//! ワールド表示名
	FText		m_DisplayName;

	//! ワールドマネージャ
	UPROPERTY(Transient)
	UNaWorldManager*	m_WM;
	//! ワールド表示管理アクター
	UPROPERTY(Transient)
	ANaWorldActor*		m_WorldActor;
	//! ワールドジェネレータ
	UPROPERTY(Transient)
	UNaWorldGenerator*	m_Generator;

	//! ホームポジション
	TArray<FIntVector>	m_HomePositions;

	// 環境情報 //
	int32	m_Temperature;		// 気温
	int32	m_Humidity;			// 湿度
	int32	m_AtmosPressure;	// 気圧

	//! 表示原点
	FIntVector			m_ViewOrigin;
	FIntVector			m_CurrentChunkPos;	// 
	int32				m_CeilZ;			// 
	ENaDirection		m_WorldDirection;	// ワールド表示方角
	//! アクティブチャンク範囲
	FIntVector			m_ChunkRange;

	// メイン地形情報
	UPROPERTY(Transient)
	TArray<UNaRegion*>				m_Regions;		// リージョン実体
	TMap<FIntVector, UNaRegion*>	m_RegionMap;	// リージョンアクセス用マップ
	TMap<FIntVector, UNaChunk*>		m_ChunkMap;		// チャンクアクセス用マップ（実体はリージョン内管理）

	// 中間データマップ
	UPROPERTY()
	TArray<FNaMapEntry>				m_MapEntries;	// マップエントリ
	UPROPERTY(Transient)
	TMap<int32, UNaMap*>			m_MapData;		// 展開中のマップ実体

	//! ワールドステージ管理エンティティ実体
	UPROPERTY()
	TArray<UNaEntity*>				m_Entities;
	//! 追加待ちエンティティ
	UPROPERTY()
	TArray<UNaEntity*>				m_LazyEntities;
	//! 次回ワールドエンティティID
	UPROPERTY()
	int32							m_NextEntityID;
	//! スポーン中のエンティティ（ステージ共通）
	UPROPERTY(Transient)
	TArray<UNaEntity*>				m_SpawnEntities;
};
