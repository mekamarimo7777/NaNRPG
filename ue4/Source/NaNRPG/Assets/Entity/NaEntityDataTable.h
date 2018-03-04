// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "Misc/NaEnums.h"
#include "Misc/NaStructs.h"

#include "NaEntityDataTable.generated.h"

// エンティティデータ
USTRUCT()
struct FNaEntityDataAsset : public FTableRowBase
{
	GENERATED_BODY()

	// 固有ID //
	UPROPERTY(EditDefaultsOnly, Category = "General")
	int32	UID;
	// 内部識別名 //
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FName	Name;
	// エンティティ種別 //
	UPROPERTY(EditDefaultsOnly, Category = "General")
	TEnumAsByte<ENaEntity::Type>	Type;

	// 表示名（このエンティティデータの名前。「タンス」とか「街の貴族」とか） //
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FText	DisplayName;
	// 使用アクター //
	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	FName	ActorName;
	// ユニーク表示名（生成されるキャラクター・オブジェクトの固有名。空白はルールに従ってランダム生成） //
	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	FText	UniqueName;

	//!
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32	AIType;

	//! エンティティサイズ
	UPROPERTY(EditDefaultsOnly, Category = "Object")
	FIntVector	EntitySize;
	//! 衝突判定
	UPROPERTY(EditDefaultsOnly, Category = "Object")
	bool		IsCollidable;
	
	//! レベル（仮）
	UPROPERTY(EditDefaultsOnly, Category = "Status")
	int32	Level;

	///// キャラクターエンティティ系 ///
	// 職業 //
	UPROPERTY(EditDefaultsOnly, Category = "Status")
	FName	Job;
	// 種族 //
	UPROPERTY(EditDefaultsOnly, Category = "Status")
	FName	Race;
	// 性別（ランダム指定時は種族ごとの出現率でランダム） //
//	UPROPERTY(EditDefaultsOnly, Category = "Status")
//	TEnumAsByte<ENaGender::Type>	Gender;
	// グレード（生成される強度） //
	UPROPERTY(EditDefaultsOnly, Category = "Status")
	int32	Grade;


	/// オブジェクトエンティティ系 ///

	//! デフォルトイベント
	UPROPERTY(EditDefaultsOnly, Category = "Event")
	FName	DefaultEvent;

	FNaEntityDataAsset()
	: IsCollidable( true )
	{
	}
};
