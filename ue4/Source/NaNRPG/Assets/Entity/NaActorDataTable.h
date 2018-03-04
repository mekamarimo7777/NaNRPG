// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "Misc/NaStructs.h"

UENUM()
namespace ENaActorVisual
{
	enum Type
	{
		//! スタティックメッシュ
		StaticMesh,
		//! スケルタルメッシュ
		SkeletalMesh,
		//! ビルボード
		Billboard,

		ENUM_MAX	UMETA(Hidden)
	};
}

#include "NaActorDataTable.generated.h"

class ANaActorBase;

//! スプライトデータ定義
//USTRUCT()

//! 3Dアニメーションデータ定義
//USTRUCT()

//! アクター表示データ定義
USTRUCT()
struct FNaActorDataAsset : public FTableRowBase
{
	GENERATED_BODY()

	// 固有ID //
	UPROPERTY(EditDefaultsOnly, Category = General)
	int32	UID;
	// 内部識別名 //
	UPROPERTY(EditDefaultsOnly, Category = General)
	FName	Name;

	// アクタークラス
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	TSubclassOf<ANaActorBase>	ActorClass;
	// スタティックメッシュ
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	TAssetPtr<UStaticMesh>		StaticMesh;
	// スケルタルメッシュ
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	TAssetPtr<USkeletalMesh>	SkeletalMesh;
	// 2Dビルボードキャラ用
//	UPROPERTY(EditDefaultsOnly, Category = Visual)
//	TAssetPtr<UPaper2DSprite>	Sprite;

	//! 表示スケール
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	FVector	Scale;

	FNaActorDataAsset()
	: Scale( 1.0f, 1.0f, 1.0f )
	{
	}
};
