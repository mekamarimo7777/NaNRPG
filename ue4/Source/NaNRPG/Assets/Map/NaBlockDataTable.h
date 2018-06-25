// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "Misc/NaStructs.h"

UENUM()
namespace ENaBlockPhase
{
	enum Type
	{
		//! 固体
		Solid,
		//! 液体
		Liquid,
		//! 気体
		Gas,
		//! プラズマ
		Plasma,

		ENUM_MAX	UMETA(Hidden)
	};
}

#include "NaBlockDataTable.generated.h"

// マップマテリアル定義 //
USTRUCT()
struct FNaBlockMaterialAsset : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = General)
	int32	UID;
	UPROPERTY(EditDefaultsOnly, Category = General)
	FName	Name;

	UPROPERTY(EditDefaultsOnly, Category = Rendering, Meta = (AllowedClasses = "MaterialInterface"))
	FStringAssetReference	Material;
};

// マップブロック定義 //
USTRUCT()
struct FNaBlockDataAsset : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	//! ID（0～254まで。255以上は無効）
	UPROPERTY(EditDefaultsOnly, Category = General)
	int32	UID;
	UPROPERTY(EditDefaultsOnly, Category = General)
	FName	Name;
	UPROPERTY(EditDefaultsOnly, Category = General)
	FText	DisplayName;

	//! 描画の有無
	UPROPERTY(EditDefaultsOnly, Category = Rendering)
	bool	IsRender;
	//! メッシュ形状
	UPROPERTY(EditDefaultsOnly, Category = Rendering)
	int32	MeshType;
	//! マップマテリアルID
	UPROPERTY(EditDefaultsOnly, Category = Rendering)
	int32	MaterialID;
	//! マテリアルタイルインデックス
	UPROPERTY(EditDefaultsOnly, Category = Rendering)
	int32	TileID[ENaMapBlockFace::ENUM_MAX];

	//! 相
	UPROPERTY(EditDefaultsOnly, Category = Attributes)
	TEnumAsByte<ENaBlockPhase::Type>	Phase;
	//! 透過ブロック判定
	UPROPERTY(EditDefaultsOnly, Category = Attributes)
	bool	IsOpaque;
	//! 登攀難易度
	UPROPERTY(EditDefaultsOnly, Category = Attributes)
	bool	ClimbDiff;

	FNaBlockDataAsset()
	: UID( 0 )
	, IsRender( true )
	{
	}
};
