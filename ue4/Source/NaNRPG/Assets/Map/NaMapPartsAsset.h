// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

//#include "Assets/Entity/NaEntityAsset.h"

#include "NaMapPartsAsset.generated.h"

USTRUCT()
struct FNaMapPartsCell
{
	GENERATED_BODY()

	UPROPERTY()
	uint8	BlockID;
	UPROPERTY()
	uint8	Height[4];

	FNaMapPartsCell()
	: BlockID( 0 )
	{}
};

USTRUCT()
struct FNaEntityEntry
{
	GENERATED_BODY()

	//! 配置エンティティ
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FName			EntityID;

	//! 位置
	UPROPERTY(EditDefaultsOnly, Category = "Location")
	FIntVector		Position;
	//! 位置（ランダム範囲）
	UPROPERTY(EditDefaultsOnly, Category = "Location")
	FIntVector		PositionRange;
	//! 向き
	UPROPERTY(EditDefaultsOnly, Category = "Location")
	ENaDirection	Direction;

	//! 所属ステージ
	UPROPERTY(EditDefaultsOnly, Category = "Generation")
	TEnumAsByte<ENaEntityStage::Type>	Stage;

	//! イベントアセット名
	UPROPERTY(EditDefaultsOnly, Category = "Event")
	FName					EventID;
	//! パラメータリスト
	UPROPERTY(EditDefaultsOnly, Category = "Event")
	TMap<FName, FString>	Params;

};

/**
 * 
 */
UCLASS()
class NANRPG_API UNaMapPartsAsset : public UObject
{
	GENERATED_BODY()
	
public:
	static UNaMapPartsAsset* CreateDynamic(int32 type);

	//
	void	CreateDynamicParts(int32 type);
	//!
	void	Resize(FIntVector newSize);

	//
	FORCEINLINE FNaMapPartsCell*	GetCell(int32 x, int32 y, int32 z)	{return &m_Cells[x + y * Size.X + z * Size.X * Size.Y];}
	FORCEINLINE FNaMapPartsCell*	GetCell(FIntVector pos)				{return GetCell( pos.X, pos.Y, pos.Z );}

	FNaMapPartsCell*	GetWrapCell(int32 x, int32 y, int32 z);

public:
	UPROPERTY(EditAnywhere, Category = "General")
	FIntVector	Size;			// パーツサイズ（ブロック単位） //

	// 拡大用パラメータ（9スライス的な） //
	UPROPERTY()
	int32		BoxOffsetX[2];	// 拡大配置時の固定Xマージン //
	UPROPERTY()
	int32		BoxOffsetY[2];	// 拡大配置時の固定Yマージン //
	UPROPERTY()
	int32		BoxOffsetZ[2];	// 拡大配置時の固定Zマージン //

	// エンティティリスト //
	UPROPERTY()
	TArray<FNaEntityEntry>	Entities;		//  //

protected:
	//! セル情報
	UPROPERTY()
	TArray<FNaMapPartsCell>	m_Cells;
};
