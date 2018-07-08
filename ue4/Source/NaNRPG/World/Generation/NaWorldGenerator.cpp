// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaWorldGenerator.h"

#include "NaNoise.h"

#include "World/NaWorld.h"
#include "World/NaRegion.h"
#include "World/NaChunk.h"

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//
UNaWorldGenerator::UNaWorldGenerator()
{

}

//! アセット設定
void UNaWorldGenerator::SetWorldAsset( UNaWorldAsset* asset )
{
	m_Asset	= asset;
}

//! チャンク生成
void UNaWorldGenerator::GenerateChunk( UNaChunk* chunk )
{
	//! 生成対象
	m_Chunk	= chunk;

	GenerateBiome();

	//! 地形生成
	switch ( m_Asset->WorldTerrain ){
	case ENaTerrainMode::Normal:
		MakeTerrain( chunk );
		break;
	case ENaTerrainMode::Flat:
		MakeFlatland( chunk );
		break;
	}

	//! マップ反映
	CopyMapData( chunk );
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//! 
void UNaWorldGenerator::MakeTerrain( UNaChunk* chunk )
{
	FIntVector			wpos,tpos,bpos;
	FNaWorldBlockWork	block;

	GenerateHeightMap();

	wpos	= chunk->GetPositionInWorld();
	tpos.X	= wpos.X << 4;

	bpos	= chunk->GetPosition() * 16;

	// セル生成 //
	for ( int32 ix = 0; ix < UNaChunk::DIM_X; ++ix, ++tpos.X ){
		//
		tpos.Y	= wpos.Y << 4;

		for ( int32 iy = 0; iy < UNaChunk::DIM_Y; ++iy, ++tpos.Y ){
			int32	height;

			//
			tpos.Z	= wpos.Z << 4;

			height	= m_HeightMap[ix + (iy << 4)] / 4;

			for ( int32 iz = 0; iz < UNaChunk::DIM_Z; ++iz, ++tpos.Z ){
				int32	idx = CELL_OFS(ix, iy, iz);
				int32	h;

				h	= (height >> 1) + 48;

				if ( tpos.Z < h ){
					if ( tpos.Z < 0 ){
						block.BlockID	= 2;
					}
					else {
						block.BlockID	= 1;
					}
					block.BlockParam	= 0;

					if ( tpos.Z == h - 1 && !(height & 0x1) ){
						block.MetaData.Height[0]	= 128;
					}
					else {
						block.MetaData.Height[0]	= 255;
					}
					/*
					if ( tpos.Z == h ){
						block.MetaData.Height[0]	= ((height & 0x1) << 7);
					}
					else {
						block.MetaData.Height[0]	= 255;
					}
					*/

					block.MetaData.Height[1]	= block.MetaData.Height[0];
					block.MetaData.Height[2]	= block.MetaData.Height[0];
					block.MetaData.Height[3]	= block.MetaData.Height[0];
//					m_Blightness[idx >> 1]		= 0;

					block.VisibleFace	= 0;

					chunk->SetBlock( idx, block );
				}
				else {
					block.BlockID			= 0;
					block.MetaData.Value	= 0;
					block.VisibleFace		= 0;
					chunk->SetBlock( idx, block );
				}
			}
		}
	}
}

//! 平坦な地形生成
void UNaWorldGenerator::MakeFlatland( UNaChunk* chunk )
{
	FIntVector			wpos,tpos,bpos;
	FNaWorldBlockWork	block;

	wpos	= chunk->GetPositionInWorld();
	tpos.X	= wpos.X << 4;

	bpos	= chunk->GetPosition() * 16;

	// セル生成 //
	for ( int32 ix = 0; ix < UNaChunk::DIM_X; ++ix, ++tpos.X ){
		//
		tpos.Y	= wpos.Y << 4;

		for ( int32 iy = 0; iy < UNaChunk::DIM_Y; ++iy, ++tpos.Y ){
			//
			tpos.Z	= wpos.Z << 4;

			for ( int32 iz = 0; iz < UNaChunk::DIM_Z; ++iz, ++tpos.Z ){
				int32	idx = CELL_OFS(ix, iy, iz);
				int32	h;

				h	= 48;

				if ( tpos.Z < h ){
					block.BlockID		= 1;
					block.BlockParam	= 0;

					block.MetaData.Height[0]	= 255;
					block.MetaData.Height[1]	= block.MetaData.Height[0];
					block.MetaData.Height[2]	= block.MetaData.Height[0];
					block.MetaData.Height[3]	= block.MetaData.Height[0];
					block.VisibleFace	= 0;
					chunk->SetBlock( idx, block );
				}
				else {
					block.BlockID			= 0;
					block.MetaData.Value	= 0;
					block.VisibleFace		= 0;
					chunk->SetBlock( idx, block );
				}
			}
		}
	}
}

//! ハイトマップ生成
void UNaWorldGenerator::GenerateHeightMap()
{
	FNaNoise	noise( 0 );
	int32		idx = 0;
	FIntVector	bpos;

	m_HeightMap.SetNum( 16 * 16 );
	bpos	= m_Chunk->GetPositionInWorld() * 16;

	for ( int32 x = 0; x < 16; ++x ){
		for ( int32 y = 0; y < 16; ++y ){
			idx	= x + (y << 4);
			m_HeightMap[idx]	= noise.octaveNoise0_1( (x + bpos.X) / 64.0f, (y + bpos.Y) / 64.0f, 6 ) * 255;
		}
	}
}

//! バイオーム生成
void UNaWorldGenerator::GenerateBiome()
{
	FNaNoise	noise( 0 );
	int32		idx = 0;
	FIntVector	bpos;

	m_Biome.SetNum( 16 * 16 );
	bpos	= m_Chunk->GetPositionInWorld() * 16;

	for ( int32 x = 0; x < 16; ++x ){
		for ( int32 y = 0; y < 16; ++y ){
			idx	= x + (y << 4);
//			m_HeightMap[idx]	= noise.octaveNoise0_1( (x + bpos.X) / 64.0f, (y + bpos.Y) / 64.0f, 6 ) * 255;
		}
	}
}

//! マップデータ書き込み
void UNaWorldGenerator::CopyMapData( UNaChunk* chunk )
{
	UNaWorld*		naw = chunk->GetRegion()->GetNaWorld();
	TArray<UNaMap*>	maps;
	UNaChunk*		mapChunk;
	FIntVector		cpos,tpos;

	cpos	= chunk->GetPositionInWorld();
	naw->FindMap( cpos, maps );

	for ( auto& it : maps ){
		tpos		= cpos - it->GetLocation();
		mapChunk	= it->GetChunk( tpos );

		if ( mapChunk ){
			for ( int32 i = 0; i < UNaChunk::ELEMENTS; ++i ){
				FNaWorldBlockWork	work;

				if ( mapChunk->GetBlock( i, work ) ){
					chunk->SetBlock( i, work );
				}
			}
		}
	}
}
