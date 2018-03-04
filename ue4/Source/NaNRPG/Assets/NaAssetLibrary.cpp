// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaAssetLibrary.h"

#include "Singleton/NaGameInstance.h"

//
UNaAssetLibrary* UNaAssetLibrary::Get()
{
#if 1
	static bool	bInitialized = false;

	UNaAssetLibrary*	lib = GetMutableDefault<UNaAssetLibrary>();

	if ( !bInitialized ){
		lib->Initialize();
		bInitialized	= true;
	}

	return lib;
#else
	UNaGameInstance*	gi = Cast<UNaGameInstance>( UGameplayStatics::GetGameInstance( GWorld ) );

	if ( gi ){
		return gi->GetALib();
	}
	else {
		return nullptr;
	}
#endif
}

//!
void UNaAssetLibrary::Initialize()
{
	//! アクター
	GatherDataTableAsset<FNaActorDataAsset>( m_LibActorData, "/Game/Assets/Data/ActorData", m_ActorDataMap );
	
	//! エンティティ
	GatherDataTableAsset<FNaEntityDataAsset>( m_LibEntityData, "/Game/Assets/Data/EntityData", m_EntityDataMap );
	//! 種族
	GatherDataTableAsset<FNaRaceDataAsset>( m_LibRaceData, "/Game/Assets/Data/RaceData", m_RaceDataMap );

	//! アイテム
	GatherDataTableAsset<FNaItemDataAsset>( m_LibItemData, "/Game/Assets/Data/ItemData", m_ItemDataMap );
	//! 素材
	GatherDataTableAsset<FNaMatterDataAsset>( m_LibMatterData, "/Game/Assets/Data/MatterData", m_MatterDataMap );

	//! マップ関連
	GatherBlockAsset( "/Game/Assets/Data/Map/BlockData" );
	GatherBlockMaterialAsset( "/Game/Assets/Data/Map/BlockMaterial" );

	//! ワールド
	GatherDataAsset<UNaWorldAsset>( m_LibWorldAssets, "/Game/Assets/Data/WorldData", m_WorldAssets );

	//! イベント
	GatherDataAsset<UNaEventAsset>( m_LibEventAssets, "/Game/Assets/Event", m_EventAssets );
}

//!
FNaActorDataAsset* UNaAssetLibrary::FindActorAsset( FName uid ) const
{
	return m_ActorDataMap.FindRef( uid );
}

//! エンティティ定義取得
FNaEntityDataAsset*	UNaAssetLibrary::FindEntityAsset( FName uid ) const
{
	return m_EntityDataMap.FindRef( uid );
}

//!
FNaRaceDataAsset* UNaAssetLibrary::FindRaceAsset( FName uid ) const
{
	return m_RaceDataMap.FindRef( uid );
}

//!
FNaItemDataAsset* UNaAssetLibrary::FindItemAsset( FName uid ) const
{
	return m_ItemDataMap.FindRef( uid );
}

//!
FNaBlockDataAsset* UNaAssetLibrary::FindBlockAsset( int32 uid ) const
{
	return m_BlockDataList[uid];
}

//!
FNaBlockMaterialAsset* UNaAssetLibrary::FindBlockMaterialAsset( int32 uid ) const
{
	return m_BlockMaterialDataList[uid];
}

//! ワールドアセット参照
UNaWorldAsset* UNaAssetLibrary::FindWorldAsset( FName uid ) const
{
	TAssetPtr<UNaWorldAsset>*	asset;

	asset	= m_WorldAssets.Find( uid );

	if ( asset ){
		return asset->LoadSynchronous();
	}
	else {
		return nullptr;
	}
}

//! イベントアセット参照
UNaEventAsset* UNaAssetLibrary::FindEventAsset( FName uid ) const
{
	TAssetPtr<UNaEventAsset>*	asset;

	asset	= m_EventAssets.Find( uid );

	if ( asset ){
		return asset->LoadSynchronous();
	}
	else {
		return nullptr;
	}
}

//====================================================================================================

//! データテーブルアセットデータ収集
template<typename T>
void UNaAssetLibrary::GatherDataTableAsset( UObjectLibrary*& lib, FString path, TMap<FName, T*>& outMap )
{
	if ( !lib ){
		lib	= UObjectLibrary::CreateLibrary( UDataTable::StaticClass(), false, GIsEditor );
		lib->AddToRoot();
	}
	lib->LoadAssetDataFromPath( path );

	if ( true ){
		TArray<FAssetData>	assets;

		lib->LoadAssetsFromAssetData();
		lib->GetAssetDataList( assets );

		for ( auto& it : assets ){
			UDataTable*		table = Cast<UDataTable>( it.GetAsset() );
			TArray<uint8*>	values;

			if ( table->RowStruct != T::StaticStruct() ){
				continue;
			}

			table->RowMap.GenerateValueArray( values );

			for ( auto& it2 : values ){
				T*	data = reinterpret_cast<T*>( it2 );

				if ( !outMap.Contains( data->Name ) ){
					outMap.Add( data->Name, data );
				}
			}
		}
	}
}

//! アセットデータ収集
template<typename T>
void UNaAssetLibrary::GatherDataAsset( UObjectLibrary*& lib, FString path, TMap<FName, TAssetPtr<T> >& outMap )
{
	//! 
	if ( !lib ){
		lib	= UObjectLibrary::CreateLibrary( T::StaticClass(), false, GIsEditor );
		lib->AddToRoot();
	}
	lib->LoadAssetDataFromPath( path );

	if ( true ){
		TArray<FAssetData>	assets;

		lib->LoadAssetsFromAssetData();
		lib->GetAssetDataList( assets );

		for ( auto& it : assets ){
			const FString*	pStr = it.TagsAndValues.Find( GET_MEMBER_NAME_CHECKED( T, UID ) );

			if ( pStr ){
				TAssetPtr<T>	ptr;

				ptr	= it.ToSoftObjectPath();
				outMap.Add( FName( **pStr ), ptr );
			}
		}
	}
}

//! ブロックデータ収集
void UNaAssetLibrary::GatherBlockAsset( FString path )
{
	TArray<FAssetData>	assets;

	if ( !m_LibBlockData ){
		m_LibBlockData	= UObjectLibrary::CreateLibrary( UDataTable::StaticClass(), false, GIsEditor );
		m_LibBlockData->AddToRoot();
	}
	m_LibBlockData->LoadAssetDataFromPath( path );

	m_LibBlockData->LoadAssetsFromAssetData();
	m_LibBlockData->GetAssetDataList( assets );

	for ( auto& it : assets ){
		UDataTable*		table = Cast<UDataTable>( it.GetAsset() );
		TArray<uint8*>	values;

		if ( table->RowStruct != FNaBlockDataAsset::StaticStruct() ){
			continue;
		}

		table->RowMap.GenerateValueArray( values );

		for ( auto& it2 : values ){
			FNaBlockDataAsset*	data = reinterpret_cast<FNaBlockDataAsset*>( it2 );

			if ( data->UID >= m_BlockDataList.Num() ){
				m_BlockDataList.SetNum( data->UID + 1 );
			}
			m_BlockDataList[data->UID]	= data;
		}
	}
}

//! ブロックマテリアルデータ収集
void UNaAssetLibrary::GatherBlockMaterialAsset( FString path )
{
	TArray<FAssetData>	assets;

	if ( !m_LibBlockMaterialData ){
		m_LibBlockMaterialData	= UObjectLibrary::CreateLibrary( UDataTable::StaticClass(), false, GIsEditor );
		m_LibBlockMaterialData->AddToRoot();
	}
	m_LibBlockMaterialData->LoadAssetDataFromPath( path );

	m_LibBlockMaterialData->LoadAssetsFromAssetData();
	m_LibBlockMaterialData->GetAssetDataList( assets );

	for ( auto& it : assets ){
		UDataTable*		table = Cast<UDataTable>( it.GetAsset() );
		TArray<uint8*>	values;

		if ( table->RowStruct != FNaBlockMaterialAsset::StaticStruct() ){
			continue;
		}

		table->RowMap.GenerateValueArray( values );

		for ( auto& it2 : values ){
			FNaBlockMaterialAsset*	data = reinterpret_cast<FNaBlockMaterialAsset*>( it2 );

			if ( data->UID >= m_BlockMaterialDataList.Num() ){
				m_BlockMaterialDataList.SetNum( data->UID + 1 );
			}
			m_BlockMaterialDataList[data->UID]	= data;
		}
	}
}
