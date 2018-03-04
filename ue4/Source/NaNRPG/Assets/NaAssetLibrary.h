// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc/NaStructs.h"

#include "Blueprint/UserWidget.h"

#include "Map/NaBlockDataTable.h"

#include "Item/NaItemDataTable.h"
#include "Item/NaMatterDataTable.h"

#include "Skill/NaSkillDataTable.h"

#include "Entity/NaActorDataTable.h"
#include "Entity/NaEntityDataTable.h"
#include "Entity/NaRaceDataTable.h"

#include "Map/NaWorldAsset.h"

#include "Event/NaEventAsset.h"

#include "NaAssetLibrary.generated.h"

/**
 * アセットデータ
 */
UCLASS()
class NANRPG_API UNaAssetLibrary : public UObject
{
	GENERATED_BODY()

public:
	//
	static UNaAssetLibrary*	Get();

public:
	//! 初期化
	void	Initialize();

	//!
	FNaActorDataAsset*	FindActorAsset( FName uid ) const;
	//!
	FNaEntityDataAsset*	FindEntityAsset( FName uid ) const;
	//!
	FNaRaceDataAsset*	FindRaceAsset( FName uid ) const;
	//!
	FNaItemDataAsset*	FindItemAsset( FName uid ) const;

	//!
	FNaBlockDataAsset*		FindBlockAsset( int32 uid ) const;
	//!
	FNaBlockMaterialAsset*	FindBlockMaterialAsset( int32 uid ) const;

	//! ワールドアセット参照
	UNaWorldAsset*		FindWorldAsset( FName uid ) const;
	//! イベントアセット参照
	UNaEventAsset*		FindEventAsset( FName uid ) const;

	//!
	const TMap<FName, FNaItemDataAsset*>&	GetItemList() const		{ return m_ItemDataMap; }
	//!
	const TArray<FNaBlockDataAsset*>&		GetMapBlockList() const	{ return m_BlockDataList; }

protected:
	//! 各アセットデータ収集
	template<typename T>
	void	GatherDataTableAsset( UObjectLibrary*& lib, FString path, TMap<FName, T*>& map );
	//! 各アセットデータ収集
	template<typename T>
	void	GatherDataAsset( UObjectLibrary*& lib, FString path, TMap<FName, TAssetPtr<T> >& outArray );

	//! ブロックデータ収集
	void	GatherBlockAsset( FString path );
	//! ブロックマテリアルデータ収集
	void	GatherBlockMaterialAsset( FString path );

protected:
	//! アクターデータ
	UPROPERTY()
	UObjectLibrary*						m_LibActorData;
	TMap<FName, FNaActorDataAsset*>		m_ActorDataMap;

	//! エンティティデータ
	UPROPERTY()
	UObjectLibrary*						m_LibEntityData;
	TMap<FName, FNaEntityDataAsset*>	m_EntityDataMap;
	//! 種族データ
	UPROPERTY()
	UObjectLibrary*						m_LibRaceData;
	TMap<FName, FNaRaceDataAsset*>		m_RaceDataMap;
	//! 職業データ

	//! アイテムデータ
	UPROPERTY()
	UObjectLibrary*						m_LibItemData;
	TMap<FName, FNaItemDataAsset*>		m_ItemDataMap;
	//! 素材データ
	UPROPERTY()
	UObjectLibrary*						m_LibMatterData;
	TMap<FName, FNaMatterDataAsset*>	m_MatterDataMap;

	//! スキルデータ
	//! スペルデータ

	//! マップアセット
	UPROPERTY()
	UObjectLibrary*						m_LibBlockData;
	TArray<FNaBlockDataAsset*>			m_BlockDataList;
	UPROPERTY()
	UObjectLibrary*						m_LibBlockMaterialData;
	TArray<FNaBlockMaterialAsset*>		m_BlockMaterialDataList;

	//! ワールドアセット
	UPROPERTY()
	UObjectLibrary*									m_LibWorldAssets;
	UPROPERTY()
	mutable TMap<FName, TAssetPtr<UNaWorldAsset> >	m_WorldAssets;

	//! イベントアセット
	UPROPERTY()
	UObjectLibrary*									m_LibEventAssets;
	UPROPERTY()
	mutable TMap<FName, TAssetPtr<UNaEventAsset> >	m_EventAssets;

public:

public:
};
