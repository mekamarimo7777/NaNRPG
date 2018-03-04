// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaWorldGenerator.h"

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

//! チャンク生成
void UNaWorldGenerator::GenerateChunk( UNaChunk* chunk )
{
	//! 地形生成
	MakeTerrain( chunk );
	//MakeFlatland( chunk );

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

			height	= chunk->GetRegion()->GetHeightMapValue( bpos.X + ix, bpos.Y + iy ) / 4;

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
			int32	height;

			//
			tpos.Z	= wpos.Z << 4;

			height	= chunk->GetRegion()->GetHeightMapValue( bpos.X + ix, bpos.Y + iy ) / 4;

			for ( int32 iz = 0; iz < UNaChunk::DIM_Z; ++iz, ++tpos.Z ){
				int32	idx = CELL_OFS(ix, iy, iz);
				int32	h;

				h	= 0;

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
			}
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

				mapChunk->GetBlock( i, work );
				chunk->SetBlock( i, work );
			}
		}
	}
}
