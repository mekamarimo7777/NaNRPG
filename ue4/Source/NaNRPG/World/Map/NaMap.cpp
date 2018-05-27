// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaMap.h"

#include "Actor/Entity/NaActorBase.h"

#include "Assets/NaAssetLibrary.h"
#include "Assets/Map/NaMapAsset.h"

#include "Entity/INaEntityFactory.h"

//! マップ生成
void UNaMap::Create( UNaWorld* naw, int32 uid, FIntVector pos, const UNaMapAsset* asset )
{
	m_MapID			= uid;
	m_MapLocation	= pos;
	m_MapSize		= asset->m_MapSize;

	//! マップチャンク生成
	{
		UNaChunk*	chunk;

		for ( auto& it : asset->m_Chunks ){
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

		for ( auto& it : asset->m_Entities ){
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

// @deprecated アセットからインスタンス化 //
void UNaMap::Instantiate( UNaMapAsset* src )
{
	m_MapSize	= src->m_MapSize;

	FIntVector	cmin,cmax;

	// チャンク生成 //
	for ( auto& it : src->m_Parts ){
		cmin.X	= it.Position.X >> 4;
		cmin.Y	= it.Position.Y >> 4;
		cmin.Z	= it.Position.Z >> 4;
		cmax.X	= (it.Position.X + it.Size.X - 1) >> 4;
		cmax.Y	= (it.Position.Y + it.Size.Y - 1) >> 4;
		cmax.Z	= (it.Position.Z + it.Size.Z - 1) >> 4;

		for ( int32 x = cmin.X; x <= cmax.X; ++x ){
			for ( int32 y = cmin.Y; y <= cmax.Y; ++y ){
				for ( int32 z = cmin.Z; z <= cmax.Z; ++z ){
					UNaChunk*	chunk;
					FIntVector	cpos = FIntVector( x, y, z );

					chunk	= m_ChunkMap.FindRef( cpos );
					if ( !chunk ){
						chunk	= NewObject<UNaChunk>();
						chunk->Initialize( nullptr, cpos );
						m_Chunks.Add( chunk );
						m_ChunkMap.Add( cpos, chunk );
					}

					CopyPartsToChunk( &it, chunk );

					chunk	= nullptr;
				}
			}
		}
	}

	// 
}

// パーツ情報からチャンクに変換 //
void UNaMap::CopyPartsToChunk( FNaMapPartsEntry* parts, UNaChunk* chunk )
{
	FNaWorldBlockWork	work;
	FIntVector	pbase,ppos;

	pbase	= parts->Position - chunk->GetPosition() * 16;

	for ( int32 x = 0; x < 16; ++x ){
		ppos.X	= x - pbase.X;
		if ( ppos.X < 0 ){
			continue;
		}
		else if ( ppos.X >= parts->Size.X ){
			break;
		}

		for ( int32 y = 0; y < 16; ++y ){
			ppos.Y	= y - pbase.Y;
			if ( ppos.Y < 0 ){
				continue;
			}
			else if ( ppos.Y >= parts->Size.Y ){
				break;
			}

			for ( int32 z = 0; z < 16; ++z ){
				ppos.Z	= z - pbase.Z;
				if ( ppos.Z < 0 ){
					continue;
				}
				else if ( ppos.Z >= parts->Size.Z ){
					break;
				}

//				work.BlockID	= parts->TestIdx;
				work.BlockID	= 1;
				chunk->SetBlock( x, y, z, work );
			}
		}
	}
}
