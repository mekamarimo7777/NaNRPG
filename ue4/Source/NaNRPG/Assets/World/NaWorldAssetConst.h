// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaWorldAssetConst.generated.h"

//! 地形生成モード
UENUM()
enum class ENaTerrainMode
{
	//! 基本地形
	Normal,
	//! フラット
	Flat,

	ENUM_MAX	UMETA(Hidden)
};

//! バイオーム生成モード
UENUM()
enum class ENaBiomeMode
{
	//! 
	Worm,

	ENUM_MAX	UMETA(Hidden)
};
