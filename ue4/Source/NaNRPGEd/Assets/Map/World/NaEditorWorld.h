// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Assets/World/NaMapAsset.h"

#include "World/NaWorld.h"

#include "NaEditorWorld.generated.h"

/**
 * 
 */
UCLASS()
class NANRPGED_API UNaEditorWorld : public UNaWorld
{
	GENERATED_BODY()

public:
	// コンストラクタ
	UNaEditorWorld();

	//! マップアセットから構築
	void		SetupFromMap( UNaMapAsset* asset );
	//! マップサイズ変更
	void		ResizeWorld( FIntVector size );
	//! 
	FIntVector	GetWorldSize();

	//! ブロック更新
	virtual void	SetBlock( FIntVector pos, FNaWorldBlockWork& block ) override;

	//! 再評価
	virtual void	Evaluate() override;

	//! ランダム地形生成
	void	GenerateRandomTerrain();

	//! リージョンデータ読み込み
	virtual bool	ReadRegionData( FIntVector pos, TArray<uint8>& outVal ) override;
	//! リージョンデータ書き込み
	virtual bool	WriteRegionData( FIntVector pos, const TArray<uint8>& inVal ) override;

protected:
	//! 
	UNaChunk*	CreateChunk( FIntVector pos );
	//! チャンク取得
	UNaChunk*	FindChunk( FIntVector pos );

	//! リージョン取得
	UNaRegion*	FindRegion( FIntVector pos );
	//! リージョンオープン
	UNaRegion*	OpenRegion( FIntVector pos );

public:
	
protected:
	//! 管理アセット
	UNaMapAsset*	m_MapAsset;

	//! 内部リージョンデータ
	TMap<FIntVector, TArray<uint8> >	m_RegionData;

};
