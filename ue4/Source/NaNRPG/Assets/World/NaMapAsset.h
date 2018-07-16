// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Assets/NaDataAsset.h"

#include "World/NaWorldDefinition.h"

#include "NaMapPartsAsset.h"

#include "NaMapAsset.generated.h"

//! ランダムマップ部屋生成種別
UENUM()
enum class ENaRandomMapRoom
{
	//! 基本生成ルーチン
	Normal,
	//! 円状配置
	Circle,

	ENUM_MAX	UMETA(Hidden)
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

// ランダムマップ用パーツ
USTRUCT()
struct FNaRandomMapParts
{
	GENERATED_BODY()

	//! パーツアセットID
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FName		ID;
	//! 適用対象（部屋or通路）
	UPROPERTY(EditDefaultsOnly, Category = "General")
	bool		IsRoom;
	//! 使用上限（-1で無制限）
	UPROPERTY(EditDefaultsOnly, Category = "General")
	int32		Limit;
};

// ランダムマップ部屋生成パラメータ
USTRUCT()
struct FNaRandomMapRoomParam
{
	GENERATED_BODY()

	//! 種別
	UPROPERTY(EditDefaultsOnly, Category = "General")
	ENaRandomMapRoom	Type;
	//! 生成範囲幅
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FIntVector			Range;

	//! 分布範囲
	UPROPERTY(EditDefaultsOnly, Category = "Normal")
	float		Radius;
	//! ランダム生成部屋数
	UPROPERTY(EditDefaultsOnly, Category = "Normal")
	int32		RoomGen;
	//! 離散カウンタ

	//! 最小生成部屋サイズ
	UPROPERTY(EditDefaultsOnly, Category = "Room")
	FIntPoint	MinRoomSize;
	//! 最大生成部屋サイズ
	UPROPERTY(EditDefaultsOnly, Category = "Room")
	FIntPoint	MaxRoomSize;
	//! メイン部屋サイズ閾値
	UPROPERTY(EditDefaultsOnly, Category = "Room")
	FIntPoint	MainRoomSize;
};

// ランダムマップ生成パラメータ
USTRUCT()
struct FNaRandomMapParam
{
	GENERATED_BODY()

	//! 部屋生成
	UPROPERTY(EditDefaultsOnly, Category = "General")
	TArray<FNaRandomMapRoomParam>	RoomProcess;
};

/**
 * マップデータアセット
 */
UCLASS()
class NANRPG_API UNaMapAsset : public UObject
{
	GENERATED_BODY()
	
public:
/*	// 簡易ランダムマップ生成 //
	void	CreateSimpleRandomMap( FIntVector size );
	//
	void	DivideNode( FNaMapGen* gen );
	//
	void	CreateRoom( FNaMapGen* gen );
	//
	void	CreateCorridor( FNaMapGen* gen );
	//
	void	ConnectRoom( FNaMapNode* src, FNaMapNode* dst );
	*/
	//! 
	void	ResizeMap( const FIntVector& size );
	//! 
	void	SetBlock( const FIntVector& pos, const FNaWorldBlockWork& work );

	//! 
	TArray<FNaChunkData>&	GetChunks()	{ return Chunks; }

	//! ランダムマップパラメータ取得
	const FNaRandomMapParam*	GetRandomMapParam() const	{ return &RandomMap; }

protected:
	

public:
	//! マップサイズ（チャンク単位）
	UPROPERTY()
	FIntVector	MapSize;

	//! ランダム生成の使用
	UPROPERTY(EditDefaultsOnly, Category = "Generation")
	bool						UseRandomMap;
	//! ランダム生成設定
	UPROPERTY(EditDefaultsOnly, Category = "Generation")
	FNaRandomMapParam			RandomMap;
	//! ランダム生成使用部屋パーツ
	UPROPERTY(EditDefaultsOnly, Category = "Generation")
	TArray<FNaRandomMapParts>	RandomParts;

	//! 固定チャンク
	UPROPERTY()
	TArray<FNaChunkData>		Chunks;
	//! 固定配置構造物
	UPROPERTY()
	TArray<FNaMapPartsEntry>	Parts;

	//! エンティティリスト
	UPROPERTY(EditDefaultsOnly, Category = "Entity")
	TArray<FNaEntityEntry>		Entities;

	//! 一時情報

};
