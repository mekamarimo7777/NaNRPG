// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaRegion.h"

#include "NaWorld.h"

#include "Generation/NaWorldGenerator.h"
#include "Generation/NaNoise.h"

#include "Actor/Entity/NaActorBase.h"

//! 
void UNaRegion::Initialize( UNaWorld* world, FIntVector rpos )
{
	m_pWorld	= world;
	m_Location	= rpos;

	CreateHeightMap();
}

//! リージョンオープン
void UNaRegion::Open()
{
	TArray<uint8>	buff;

	if ( m_pWorld->ReadRegionData( m_Location, buff ) ){
		FMemoryReader	reader( buff );
		Serialize( reader );
	}
}

//! リージョンクローズ
void UNaRegion::Close()
{
	TArray<uint8>	buff;

	// closechunk //
//	for ( int32 i = m_Chunks.Num() - 1; i >= 0; --i ){
//		CloseChunk( m_Chunks[i] );
//	}

	//! 
	for ( auto it : m_Chunks ){
		it->ChunkToArray( buff );
		it->Close();
		m_ChunkCache.Add( it->GetPosition(), buff );
		buff.Reset();
	}
	m_Chunks.Empty();
	m_ChunkMap.Empty();

	FBufferArchive	ar;

	Serialize( ar );

	m_pWorld->WriteRegionData( m_Location, ar );
}





//
bool UNaRegion::OpenRegion( const FString& path, FIntVector rpos )
{
	m_FilePath	= FPaths::Combine( *path, *FString::Printf( TEXT("r%d,%d,%d"), rpos.X, rpos.Y, rpos.Z ) );
	m_Location	= rpos;

	if ( !FPaths::FileExists( m_FilePath ) ){
		return false;
	}

	TArray<uint8>	buff;

	if ( FFileHelper::LoadFileToArray( buff, *m_FilePath ) ){
		FMemoryReader	reader( buff );
		Serialize( reader );
	}

	return true;
}

//
bool UNaRegion::CreateRegion( const FString& path, FIntVector rpos )
{
	m_FilePath	= FPaths::Combine( *path, *FString::Printf( TEXT("r%d,%d,%d"), rpos.X, rpos.Y, rpos.Z ) );
	m_Location	= rpos;

	return true;
}

//
void UNaRegion::CloseRegion()
{
	TArray<uint8>	buff;

	// closechunk //
//	for ( int32 i = m_Chunks.Num() - 1; i >= 0; --i ){
//		CloseChunk( m_Chunks[i] );
//	}
	for ( auto it : m_Chunks ){
		it->ChunkToArray( buff );
		it->Close();
		m_ChunkCache.Add( it->GetPosition(), buff );
		buff.Reset();
	}
	m_Chunks.Empty();
	m_ChunkMap.Empty();

	FBufferArchive	ar;

	Serialize( ar );
	FFileHelper::SaveArrayToFile( ar, *m_FilePath );
}

//
UNaChunk* UNaRegion::GetChunk( FIntVector cpos )
{
	UNaChunk*	chunk;

	cpos	-= m_Location * 16;
	chunk	= m_ChunkMap.FindRef( cpos );
	if ( !chunk ){
		TArray<uint8>*	buff;

		buff	= m_ChunkCache.Find( cpos );
		if ( !buff ){
			return false;
		}

		chunk	= NewObject<UNaChunk>();
		chunk->Initialize( this, cpos );
		chunk->ArrayToChunk( *buff );

		m_Chunks.Add( chunk );
		m_ChunkMap.Add( cpos, chunk );
		m_ChunkCache.Remove( cpos );
	}

	return chunk;
}

//
UNaChunk* UNaRegion::CreateChunk( FIntVector cpos )
{
	UNaChunk*	chunk;

	cpos	-= m_Location * 16;

	chunk	= NewObject<UNaChunk>();
	chunk->Initialize( this, cpos );

	if ( m_pWorld ){
		UNaWorldGenerator*	gen = m_pWorld->GetGenerator();

		if ( gen ){
			gen->GenerateChunk( chunk );
		}
	}

	m_Chunks.Add( chunk );
	m_ChunkMap.Add( cpos, chunk );

	return chunk;
}

//
void UNaRegion::CloseChunk( UNaChunk* chunk )
{
	if ( chunk ){
		TArray<uint8>	buff;
		FIntVector		cpos = chunk->GetPosition();

		chunk->ChunkToArray( buff );
		chunk->Close();
		m_ChunkCache.Add( cpos, buff );

		m_ChunkMap.Remove( cpos );
		m_Chunks.Remove( chunk );
	}
}
void UNaRegion::CloseChunk( FIntVector cpos )
{
	CloseChunk( m_ChunkMap.FindRef( cpos ) );
}

//
void UNaRegion::RecalcVisibleFaces()
{
	for ( auto& it : m_Chunks ){
		it->RecalcRenderCells();
	}
}

//! ハイトマップ生成
void UNaRegion::CreateHeightMap()
{
	if ( m_HeightMap.Num() == 0 ){
		FNaNoise	noise(0);
		int32		idx = 0;
		FIntVector	bpos;

		m_HeightMap.SetNum( 256 * 256 );

		bpos	= m_Location * 256;

		for ( int32 x = 0; x < 256; ++x ){
			for ( int32 y = 0; y < 256; ++y ){
				idx	= x + (y << 8);
				m_HeightMap[idx]	= noise.octaveNoise0_1( (x + bpos.X) / 64.0f, (y + bpos.Y) / 64.0f, 6 ) * 255;
			}
		}
	}
}

//! ハイトマップ値取得
int32 UNaRegion::GetHeightMapValue( int32 x, int32 y )
{
	return m_HeightMap[x + (y << 8)];
}

//
void UNaRegion::Serialize( FArchive& ar )
{
	ar << m_ChunkCache;
}

// 隣接チャンク接続（相互） //
void UNaRegion::ConnectChunk( UNaChunk* chunk )
{
	const FIntVector	c_dir[6] = 
	{
		FIntVector(-1, 0, 0 ), FIntVector( 0,-1, 0 ), FIntVector( 1, 0, 0 ), FIntVector( 0, 1, 0 ),
		FIntVector( 0, 0,-1 ), FIntVector( 0, 0, 1 ),
	};
	FIntVector	v;
	UNaChunk*	neighbor;

	for ( int32 i = 0; i < 6; ++i ){
		v			= chunk->GetPositionInWorld() + c_dir[i];
		neighbor	= m_pWorld->GetChunk( v );
		if ( neighbor ){
			chunk->ConnectNeighborChunk( i, neighbor );
		}
	}
}

//
void UNaRegion::DisconnectChunk( UNaChunk* chunk )
{
	for ( int32 i = 0; i < 6; ++i ){
		chunk->DisconnectNeighborChunk( i );
	}
}
