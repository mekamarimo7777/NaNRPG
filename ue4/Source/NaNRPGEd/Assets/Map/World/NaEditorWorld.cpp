// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPGEd.h"
#include "NaEditorWorld.h"

//====================================================================================================

//! 
UNaEditorWorld::UNaEditorWorld()
{
}

//! マップアセットから構築
void UNaEditorWorld::SetupFromMap( UNaMapAsset* asset )
{
	m_MapAsset	= asset;

	SetChunkLimit( FIntVector(0, 0, 0), asset->MapSize - FIntVector( 1, 1, 1 ) );

	//! マップデータからコピー
	if ( m_MapAsset ){
		UNaChunk*	chunk;

		for ( auto& it : m_MapAsset->GetChunks() ){
			chunk	= GetChunk( it.Position );

			if ( !chunk ){
				chunk	= CreateChunk( it.Position );
			}

			if ( chunk ){
				chunk->StoreBlocks( it.ID, it.Param, it.Meta );
			}
		}
	}

	Evaluate();
}

//! マップサイズ変更
void UNaEditorWorld::ResizeWorld(FIntVector size )
{
	m_MapAsset->ResizeMap( size );

	SetChunkLimit( FIntVector(0, 0, 0), size - FIntVector( 1, 1, 1 ) );

	Evaluate();
}

//! 
FIntVector UNaEditorWorld::GetWorldSize()
{
	return m_ChunkMax - m_ChunkMin;
}

//! ブロック更新
void UNaEditorWorld::SetBlock( FIntVector pos, FNaWorldBlockWork& block )
{
	UNaChunk*	chunk;
	FIntVector	cpos;

	cpos.X	= pos.X >> 4;
	cpos.Y	= pos.Y >> 4;
	cpos.Z	= pos.Z >> 4;

	chunk	= GetChunk( cpos );
	if ( chunk ){
		FIntVector	tpos;

		tpos	= pos;
		tpos.X	-= cpos.X << 4;
		tpos.Y	-= cpos.Y << 4;
		tpos.Z	-= cpos.Z << 4;
		chunk->SetBlock( tpos, block );
	}

	if ( m_MapAsset ){
		m_MapAsset->SetBlock( pos, block );
		m_MapAsset->MarkPackageDirty();
	}
}

//! 
void UNaEditorWorld::GenerateRandomTerrain()
{
	
}

// 
void UNaEditorWorld::Evaluate()
{
	TArray<UNaChunk*>	chunks;
	UNaRegion*			region;
	UNaChunk*			chunk;
	FIntVector			tpos,rpos;

	tpos.X	= m_CurrentChunkPos.X - (m_ChunkRange.X >> 1);

	for ( int32 ix = 0; ix < m_ChunkRange.X; ++ix, ++tpos.X ){
		if ( tpos.X < m_ChunkMin.X || tpos.X > m_ChunkMax.X ){
			continue;
		}
		//
		tpos.Y	= m_CurrentChunkPos.Y - (m_ChunkRange.Y >> 1);

		for ( int32 iy = 0; iy < m_ChunkRange.Y; ++iy, ++tpos.Y ){
			if ( tpos.Y < m_ChunkMin.Y || tpos.Y > m_ChunkMax.Y ){
				continue;
			}
			//
			tpos.Z	= m_CurrentChunkPos.Z - (m_ChunkRange.Z >> 1);

			for ( int32 iz = 0; iz < m_ChunkRange.Z; ++iz, ++tpos.Z ){
				if ( tpos.Z < m_ChunkMin.Z || tpos.Z > m_ChunkMax.Z ){
					continue;
				}

				//! 
				chunk	= FindChunk( tpos );

				if ( chunk ){
					chunks.Add( chunk );
					m_ChunkMap.Remove( tpos );
				}
				else  {
					chunk	= CreateChunk( tpos );
					check( chunk );

					chunks.Add( chunk );
				}
			}
		}
	}

	// 範囲外チャンククローズ //
	for ( auto it : m_ChunkMap ){
		region = it.Value->GetRegion();
		region->CloseChunk( it.Value->GetPosition() );
	}
	m_ChunkMap.Reset();	// 一旦消す //

	for ( auto it : chunks ){
		m_ChunkMap.Add( it->GetPositionInWorld(), it );
		it->Open();
	}

	// 空リージョンクローズ //
	for ( int32 i = m_Regions.Num() - 1; i >= 0; --i ){
		region = m_Regions[i];

		if ( region->GetActiveChunkCount() == 0 ){
			region->CloseRegion();
			m_RegionMap.Remove( region->GetRegionPos() );
			m_Regions.RemoveAt( i );
		}
	}
}

//! リージョンデータ読み込み
bool UNaEditorWorld::ReadRegionData( FIntVector pos, TArray<uint8>& outVal )
{
	if ( m_RegionData.Contains( pos ) ){
		outVal	= m_RegionData.FindRef( pos );
		return true;
	}

	return false;
}
//! リージョンデータ書き込み
bool UNaEditorWorld::WriteRegionData( FIntVector pos, const TArray<uint8>& inVal )
{
	m_RegionData.Add( pos, inVal );

	return true;
}

//====================================================================================================

//! 
UNaChunk* UNaEditorWorld::CreateChunk( FIntVector pos )
{
	UNaChunk*	chunk;
	UNaRegion*	region;
	FIntVector	rpos;

	//
	rpos.X	= pos.X >> 4;
	rpos.Y	= pos.Y >> 4;
	rpos.Z	= pos.Z >> 4;
	region	= FindRegion( rpos );

	if ( !region ){
		region	= OpenRegion( rpos );

		m_Regions.Add( region );
		m_RegionMap.Add( rpos, region );
	}

	chunk	= region->GetChunk( pos );
	if ( !chunk ){
		chunk	= region->CreateChunk( pos );
	}

	return chunk;
}

//! チャンク取得
UNaChunk* UNaEditorWorld::FindChunk( FIntVector pos )
{
	return m_ChunkMap.FindRef( pos );
}

//! リージョン取得
UNaRegion* UNaEditorWorld::FindRegion( FIntVector pos )
{
	return m_RegionMap.FindRef( pos );
}

//! リージョンオープン
UNaRegion* UNaEditorWorld::OpenRegion( FIntVector pos )
{
	UNaRegion*	region;

	region	= NewObject<UNaRegion>();
	region->Initialize( this, pos );
	region->Open();

	return region;
}
