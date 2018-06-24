// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaMap.h"

#include "Actor/Entity/NaActorBase.h"

#include "Assets/NaAssetLibrary.h"
#include "Assets/Map/NaMapAsset.h"

#include "Entity/INaEntityFactory.h"

#include "../Generation/NaDungeonGenerator.h"

//! 
struct FRandomRoom
{
	
};

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//! マップ生成
void UNaMap::Create( UNaWorld* naw, int32 uid, FIntVector pos, const UNaMapAsset* asset )
{
	m_MapID			= uid;
	m_MapLocation	= pos;
	m_MapSize		= asset->MapSize;

	//! ランダムマップ生成
	if ( asset->UseRandomMap ){
		CreateRandomMap( asset );
	}

	//! マップチャンク生成
	{
		UNaChunk*	chunk;

		for ( auto& it : asset->Chunks ){
			chunk	= NewObject<UNaChunk>();
			chunk->SetPosition( it.Position );
			chunk->StoreBlocks( it.ID, it.Param, it.Meta );

			m_Chunks.Add( chunk );
			m_ChunkMap.Add( it.Position, chunk );
		}
	}

	//! エンティティ生成
	{
		UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
		FNaEntityDataAsset*	t_asset;
		FIntVector	t_pos;

		t_pos	= m_MapLocation * 16;

		for ( auto& it : asset->Entities ){
			t_asset	= alib->FindEntityAsset( it.EntityID );
			if ( t_asset ){
				UNaEntity*	entity;

				entity	= INaEntityFactory::NewEntity( t_asset->Type );
				entity->CreateFromAsset( *t_asset );
				entity->SetWorldPosition( t_pos + it.Position );
				entity->SetEvent( it.EventID );
				entity->SetEntityParameter( it.Params );
				naw->RegisterEntity( entity );
			}
		}
	}
}

//! マップ保存
void UNaMap::SaveMap( FString path )
{
	FBufferArchive	ar;
	FString			fname;

	CloseMap();

	Serialize( ar );

	fname	= FPaths::Combine( *path, *FString::Printf( TEXT("map%d.nmap"), m_MapID ) );
	FFileHelper::SaveArrayToFile( ar, *fname );
}

//! マップ読み込み
void UNaMap::LoadMap( FString path, int32 id )
{
	FString	fname;

	fname	= FPaths::Combine( *path, *FString::Printf( TEXT("map%d.nmap"), id ) );
	if ( !FPaths::FileExists( fname ) ){
		return;
	}

	TArray<uint8>	buff;

	if ( FFileHelper::LoadFileToArray( buff, *fname ) ){
		FMemoryReader	reader( buff );
		Serialize( reader );
	}
}

//
UNaChunk* UNaMap::GetChunk( FIntVector cpos )
{
	UNaChunk*	chunk;

	chunk	= m_ChunkMap.FindRef( cpos );
	if ( !chunk ){
		TArray<uint8>*	buff;

		buff	= m_ChunkCache.Find( cpos );
		if ( !buff ){
			return false;
		}

		chunk	= NewObject<UNaChunk>();
		chunk->SetPosition( cpos );
		chunk->ArrayToChunk( *buff );

		m_Chunks.Add( chunk );
		m_ChunkMap.Add( cpos, chunk );
		m_ChunkCache.Remove( cpos );
	}

	return chunk;
}

//
void UNaMap::CloseMap()
{
	TArray<uint8>	buff;

	for ( auto it : m_Chunks ){
		it->ChunkToArray( buff );
		m_ChunkCache.Add( it->GetPosition(), buff );
		buff.Reset();
	}
	m_Chunks.Empty();
	m_ChunkMap.Empty();
}

//
void UNaMap::Serialize( FArchive& ar )
{
	ar << m_ChunkCache;
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//! ランダムマップ生成
void UNaMap::CreateRandomMap( const UNaMapAsset* asset )
{
	UNaDungeonGenerator*	gen = NewObject<UNaDungeonGenerator>();

	if ( gen ){
		//! 生成
		gen->Generate( asset );

		TMap<FIntVector, UNaChunk*>	chunks	= gen->GetChunks();
		FNaWorldBlockWork	block;
		UNaChunk*			chunk;

		for ( auto& it : chunks ){
			chunk	= NewObject<UNaChunk>();
			chunk->SetPosition( it.Key );

			for ( int i = 0; i < UNaChunk::ELEMENTS; ++i ){
				it.Value->GetBlock( i, block );
				chunk->SetBlock( i, block );
			}

			m_Chunks.Add( chunk );
			m_ChunkMap.Add( it.Key, chunk );
		}
	}
}
