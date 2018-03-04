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
 * �A�Z�b�g�f�[�^
 */
UCLASS()
class NANRPG_API UNaAssetLibrary : public UObject
{
	GENERATED_BODY()

public:
	//
	static UNaAssetLibrary*	Get();

public:
	//! ������
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

	//! ���[���h�A�Z�b�g�Q��
	UNaWorldAsset*		FindWorldAsset( FName uid ) const;
	//! �C�x���g�A�Z�b�g�Q��
	UNaEventAsset*		FindEventAsset( FName uid ) const;

	//!
	const TMap<FName, FNaItemDataAsset*>&	GetItemList() const		{ return m_ItemDataMap; }
	//!
	const TArray<FNaBlockDataAsset*>&		GetMapBlockList() const	{ return m_BlockDataList; }

protected:
	//! �e�A�Z�b�g�f�[�^���W
	template<typename T>
	void	GatherDataTableAsset( UObjectLibrary*& lib, FString path, TMap<FName, T*>& map );
	//! �e�A�Z�b�g�f�[�^���W
	template<typename T>
	void	GatherDataAsset( UObjectLibrary*& lib, FString path, TMap<FName, TAssetPtr<T> >& outArray );

	//! �u���b�N�f�[�^���W
	void	GatherBlockAsset( FString path );
	//! �u���b�N�}�e���A���f�[�^���W
	void	GatherBlockMaterialAsset( FString path );

protected:
	//! �A�N�^�[�f�[�^
	UPROPERTY()
	UObjectLibrary*						m_LibActorData;
	TMap<FName, FNaActorDataAsset*>		m_ActorDataMap;

	//! �G���e�B�e�B�f�[�^
	UPROPERTY()
	UObjectLibrary*						m_LibEntityData;
	TMap<FName, FNaEntityDataAsset*>	m_EntityDataMap;
	//! �푰�f�[�^
	UPROPERTY()
	UObjectLibrary*						m_LibRaceData;
	TMap<FName, FNaRaceDataAsset*>		m_RaceDataMap;
	//! �E�ƃf�[�^

	//! �A�C�e���f�[�^
	UPROPERTY()
	UObjectLibrary*						m_LibItemData;
	TMap<FName, FNaItemDataAsset*>		m_ItemDataMap;
	//! �f�ރf�[�^
	UPROPERTY()
	UObjectLibrary*						m_LibMatterData;
	TMap<FName, FNaMatterDataAsset*>	m_MatterDataMap;

	//! �X�L���f�[�^
	//! �X�y���f�[�^

	//! �}�b�v�A�Z�b�g
	UPROPERTY()
	UObjectLibrary*						m_LibBlockData;
	TArray<FNaBlockDataAsset*>			m_BlockDataList;
	UPROPERTY()
	UObjectLibrary*						m_LibBlockMaterialData;
	TArray<FNaBlockMaterialAsset*>		m_BlockMaterialDataList;

	//! ���[���h�A�Z�b�g
	UPROPERTY()
	UObjectLibrary*									m_LibWorldAssets;
	UPROPERTY()
	mutable TMap<FName, TAssetPtr<UNaWorldAsset> >	m_WorldAssets;

	//! �C�x���g�A�Z�b�g
	UPROPERTY()
	UObjectLibrary*									m_LibEventAssets;
	UPROPERTY()
	mutable TMap<FName, TAssetPtr<UNaEventAsset> >	m_EventAssets;

public:

public:
};
