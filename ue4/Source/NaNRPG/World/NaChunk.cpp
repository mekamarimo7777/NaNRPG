// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaChunk.h"

#include "NaRegion.h"

#include "Entity/INaEntityFactory.h"

#include "Actor/Entity/NaActorBase.h"


//
void UNaChunk::Initialize( UNaRegion* region, FIntVector cpos )
{
	m_pRegion	= region;
	m_ChunkPos	= cpos;
	m_bOpen		= false;

	m_Revision	= 0;
	m_PrevCeilZ	= INT_MAX;

	SetInvalidate( true );

	FMemory::Memset( m_BlockID, 0, sizeof( m_BlockID ) );
	FMemory::Memset( m_BlockParam, 0, sizeof( m_BlockParam ) );
	FMemory::Memset( m_MetaData, 0, sizeof( m_MetaData ) );
	FMemory::Memset( m_Blightness, 0, sizeof( m_Blightness ) );
}

//
void UNaChunk::Open()
{
	if ( m_bOpen ){
		return;
	}

	RecalcRenderCells();
	m_pRegion->ConnectChunk( this );

	//! エンティティ生成
	for ( auto& it : m_Entities ){
		m_pRegion->GetNaWorld()->EnterEntity( it );
	}
}

//
void UNaChunk::Close()
{
	for ( int32 i = 0; i < 6; ++i ){
		DisconnectNeighborChunk( i );
	}

	for ( int32 i = m_Entities.Num() - 1; i >= 0; --i ){
		m_pRegion->GetNaWorld()->LeaveEntity( m_Entities[i] );
	}
	m_Entities.Reset();

	m_bOpen	= false;
}

//
FIntVector UNaChunk::GetPositionInWorld() const
{
	return m_pRegion->GetRegionPos() * 16 + m_ChunkPos;
}

//! ブロックデータ設定
void UNaChunk::SetBlock( const FIntVector& pos, const FNaWorldBlockWork& inVal )
{
	SetBlock( pos.X, pos.Y, pos.Z, inVal );
}
void UNaChunk::SetBlock( int32 x, int32 y, int32 z, const FNaWorldBlockWork& inVal )
{
	int32	idx = CELL_OFS( x, y, z );

	if ( idx >= 0 || idx < ELEMENTS ){
		SetBlock( idx, inVal );
	}
}
void UNaChunk::SetBlock( int32 idx, const FNaWorldBlockWork& inVal )
{
	m_BlockID[idx]			= inVal.BlockID;
	m_BlockParam[idx]		= inVal.BlockParam;
	m_MetaData[idx]			= inVal.MetaData;
	m_Blightness[idx >> 1]	= inVal.Blightness;

	m_VisibleFace[idx]		= inVal.VisibleFace;

	SetInvalidate( true );
}

//! ブロックデータ取得
bool UNaChunk::GetBlock( const FIntVector& pos, FNaWorldBlockWork& outVal )
{
	return GetBlock( pos.X, pos.Y, pos.Z, outVal );
}
bool UNaChunk::GetBlock( int32 x, int32 y, int32 z, FNaWorldBlockWork& outVal )
{
	int32	idx = CELL_OFS( x, y, z );

	//! 隣接も考慮する場合
	if ( x < 0 ){
		if ( m_pNeighborChunk[0] ){
			return m_pNeighborChunk[0]->GetBlock( x + 16, y, z, outVal );
		}
		return false;
	}
	else if ( x > 15 ){
		if ( m_pNeighborChunk[2] ){
			return m_pNeighborChunk[2]->GetBlock( x - 16, y, z, outVal );
		}
		return false;
	}

	if ( y < 0 ){
		if ( m_pNeighborChunk[1] ){
			return m_pNeighborChunk[1]->GetBlock( x, y + 16, z, outVal );
		}
		return false;
	}
	else if ( y > 15 ){
		if ( m_pNeighborChunk[3] ){
			return m_pNeighborChunk[3]->GetBlock( x, y - 16, z, outVal );
		}
		return false;
	}

	if ( z < 0 ){
		if ( m_pNeighborChunk[4] ){
			return m_pNeighborChunk[4]->GetBlock( x, y, z + 16, outVal );
		}
		return false;
	}
	else if ( z > 15 ){
		if ( m_pNeighborChunk[5] ){
			return m_pNeighborChunk[5]->GetBlock( x, y, z - 16, outVal );
		}
		return false;
	}

	if ( idx < 0 || idx >= ELEMENTS ){
		return false;
	}

	return GetBlock( idx, outVal );
}
bool UNaChunk::GetBlock( int32 idx, FNaWorldBlockWork& outVal )
{
	outVal.BlockID		= m_BlockID[idx];
	outVal.BlockParam	= m_BlockParam[idx];
	outVal.MetaData		= m_MetaData[idx];
	outVal.Blightness	= m_Blightness[idx >> 1];

	outVal.VisibleFace	= m_VisibleFace[idx];

	return true;
}

//
void UNaChunk::EnumValidCellIndices( TArray<int16>& outVal )
{
	outVal.Reserve( ELEMENTS );

	for ( int32 i = 0; i < ELEMENTS; ++i ){
		if ( m_BlockID[i] ){
			outVal.Add( i );
		}
	}
}

//! 
void UNaChunk::StoreBlocks( const uint8* id, const uint8* param, const uint32* meta )
{
	FMemory::Memcpy( m_BlockID, id, sizeof(m_BlockID) );
	FMemory::Memcpy( m_BlockParam, param, sizeof(m_BlockParam) );
	FMemory::Memcpy( m_MetaData, meta, sizeof(m_MetaData) );

	SetInvalidate( true );
}

// エンティティ登録 //
void UNaChunk::AttachEntity( UNaEntity* entity )
{
	if ( !m_Entities.Contains( entity ) ){
		m_Entities.Add( entity );
	}
}

// エンティティ除去 //
void UNaChunk::DetachEntity( UNaEntity* entity )
{
	m_Entities.Remove( entity );
}

// バイト列からデータ生成 //
void UNaChunk::ArrayToChunk( const TArray<uint8>& buff )
{
	FArchiveLoadCompressedProxy	proxy( buff, ECompressionFlags::COMPRESS_ZLIB );

	Serialize( proxy );
}

// チャンクをバイト列に変換 //
void UNaChunk::ChunkToArray( TArray<uint8>& outBuff )
{
	FArchiveSaveCompressedProxy	proxy( outBuff, ECompressionFlags::COMPRESS_ZLIB );

	Serialize( proxy );
}

//
void UNaChunk::Serialize( FArchive& ar )
{
	ar.Serialize( m_BlockID, sizeof(m_BlockID) );
	ar.Serialize( m_BlockParam, sizeof(m_BlockParam) );
	ar.Serialize( m_MetaData, sizeof(m_MetaData) );
	ar.Serialize( m_Blightness, sizeof(m_Blightness) );

	//! エンティティ
	{
		TEnumAsByte<ENaEntity::Type>	etype;
		int32	tmpNum;

		if ( ar.IsSaving() ){
			TArray<UNaEntity*>	entities;

			for ( auto& it : m_Entities ){
				if ( it->GetStage() == ENaEntityStage::Chunk ){
					entities.Add( it );
				}
			}

			tmpNum	= entities.Num();
			ar << tmpNum;

			for ( auto& it : entities ){
				etype	= it->GetType();
				ar << etype;
				it->Serialize( ar );
			}
		}
		else if ( ar.IsLoading() ){
			UNaEntity*	entity;

			ar << tmpNum;
			m_Entities.SetNum( tmpNum );

			for ( int32 i = 0; i < tmpNum; ++i ){
				ar << etype;
				entity	= INaEntityFactory::NewEntity( etype );
				if ( entity ){
					entity->Serialize( ar );
					m_Entities[i]	= entity;
				}
			}

			SetInvalidate( true );
		}
	}
}

//
void UNaChunk::CreateTestChunk()
{
	FIntVector	worldPos,tpos;
//	float		heightMap[16][16];

/*	{// 起伏計算 //
		float	height[4];

		height[0]	= 0;
		height[1]	= 0;
		height[2]	= 0;
		height[3]	= 0;
		CreateDisplacementMap( (float*)heightMap, 0, 0, 16, height[0], height[1], height[2], height[3] );
	}*/

	worldPos	= GetPositionInWorld();
	tpos.X		= worldPos.X << 4;

	// セル生成 //
	for ( int32 ix = 0; ix < DIM_X; ++ix, ++tpos.X ){
		//
		tpos.Y	= worldPos.Y << 4;

		for ( int32 iy = 0; iy < DIM_Y; ++iy, ++tpos.Y ){
			//
			tpos.Z	= worldPos.Z << 4;

			float	mid_height = 16;

//			mid_height	= heightMap[iy][ix];
//			mid_height	= (ix * iy) / 32.0f + (ChunkPos.X) * 7.0f;
/*			mid_height	= (ix * iy) / 32.0f;
			if ( worldPos.Y == 0 ){
				mid_height	+= worldPos.X * 7.0f;
			}*/
			switch ( m_pRegion->GetNaWorld()->GetDataID() ){
			case 0:
				mid_height	= -FMath::Abs( tpos.X * tpos.Y ) / 64.0f;
				break;
			case 1:
				mid_height	= FMath::Abs( tpos.X * tpos.Y ) / 32.0f;
				break;
			default:
				mid_height	= 2;
				break;
			}
//			mid_height	= 2;

			for ( int32 iz = 0; iz < DIM_Z; ++iz, ++tpos.Z ){
				int32	idx = CELL_OFS(ix, iy, iz);

				if ( tpos.Z < mid_height ){
//				if ( tpos.Z < 0 ){
//					m_BlockID[idx]				= 2 + (FMath::Abs( tpos.Z ) + 3) / 4;
					m_BlockID[idx]				= 1;
					m_BlockParam[idx]			= 0;
					m_MetaData[idx].Height[0]	= FMath::Clamp( (int32)((mid_height - tpos.Z) * 255), 0, 255 );
//					m_MetaData[idx].Height[0]	= 255;
					m_MetaData[idx].Height[1]	= m_MetaData[idx].Height[0];
					m_MetaData[idx].Height[2]	= m_MetaData[idx].Height[0];
					m_MetaData[idx].Height[3]	= m_MetaData[idx].Height[0];
//					m_Blightness[idx >> 1]		= 0;

					m_VisibleFace[idx]	= 0;
				}
				else {
					m_BlockID[idx]	= 0;
				}
			}
		}
	}

	// マップチャンクのコピー（適当）
	/*
	{
		UNaMap*		map;
		UNaChunk*	chunk;
		FIntVector	cpos;

		map	= m_pRegion->GetNaWorld()->GetMap( 0 );
		if ( map ){
			cpos	= GetPositionInWorld() - map->GetLocation();
			chunk	= map->GetMapChunk( cpos );
			if ( chunk ){
				for ( int32 i = 0; i < ELEMENTS; ++i ){
					if ( chunk->m_BlockID[i] != 0 ){
						m_BlockID[i]	= chunk->m_BlockID[i];
						m_MetaData[i].Height[0]	= 0;
						m_MetaData[i].Height[1]	= 0;
						m_MetaData[i].Height[2]	= 0;
						m_MetaData[i].Height[3]	= 0;
					}
				}
			}
		}
	}
	*/

	SetInvalidate( true );
}

#if 0
//
void UNaChunk::RecalcVisibleFaces()
{
	FNaWorldBlockWork	work;
	FIntVector	wpos;
	int32		x,y,z;
	int32		wz;

	wpos	= GetPositionInWorld() * 16;

	// 天井位置更新 //
	wz		= m_pRegion->GetNaWorld()->GetCeilZ();
	if ( m_Revision > 0 ){
		bool	b0,b1;

		b0	= m_PrevCeilZ > wpos.Z && m_PrevCeilZ < wpos.Z + 17;
		b1	= wz > wpos.Z && wz < wpos.Z + 17;
		if ( !(b0 ||  b1) ){
			return;
		}
	}
	m_PrevCeilZ	= wz;

	for ( int32 i = 0; i < ELEMENTS; ++i ){
		x	= i & 0xF;
		y	= (i >> SHIFT_Y) & 0xF;
		z	= (i >> SHIFT_Z) & 0xF;

		m_VisibleFace[i]	= 0;

		if ( m_BlockID[i] > 1 ){
			if ( wpos.Z + z < wz ){
				// X
				if ( GetBlock( x - 1, y, z, work ) ){
					if ( m_MetaData[i].Height[0] > work.MetaData.Height[2] && m_MetaData[i].Height[1] > work.MetaData.Height[3] ){
						m_VisibleFace[i]	|= 0x1;
					}
				}
				else {
					m_VisibleFace[i]	|= 0x1;
				}
				if ( GetBlock( x + 1, y, z, work ) ){
					if ( m_MetaData[i].Height[2] > work.MetaData.Height[0] && m_MetaData[i].Height[3] > work.MetaData.Height[1] ){
						m_VisibleFace[i]	|= 0x2;
					}
				}
				else {
					m_VisibleFace[i]	|= 0x2;
				}

				//Y
				if ( GetBlock( x, y - 1, z, work ) ){
					if ( m_MetaData[i].Height[0] > work.MetaData.Height[1] && m_MetaData[i].Height[2] > work.MetaData.Height[3] ){
						m_VisibleFace[i]	|= 0x4;
					}
				}
				else {
					m_VisibleFace[i]	|= 0x4;
				}
				if ( GetBlock( x, y + 1, z, work ) ){
					if ( m_MetaData[i].Height[1] > work.MetaData.Height[0] && m_MetaData[i].Height[3] > work.MetaData.Height[2] ){
						m_VisibleFace[i]	|= 0x8;
					}
				}
				else {
					m_VisibleFace[i]	|= 0x8;
				}

				// 上面
				if ( wpos.Z + z == wz - 1 || !GetBlock( x, y, z + 1, work ) || work.BlockID <= 1 ){
					m_VisibleFace[i]	|= 0x20;
				}
			}

			// 下面
			if ( !GetBlock( x, y, z - 1, work ) || work.BlockID <= 1 ){
				m_VisibleFace[i]	|= 0x10;
			}
		}
	}

	m_Revision++;
	SetInvalidate( true );
}
#endif

//!
void UNaChunk::RecalcRenderCells()
{
	//!
	for ( auto& it : m_RenderCells ){
		m_VisibleFace[it]	= 0;
	}
	m_RenderCells.Reset();

	//! 
	for ( int32 i = 0; i < ELEMENTS; ++i ){
		if ( m_BlockID[i] != 0 ){
			m_RenderCells.Add( i );
		}
	}

	//! 描画面計算
	for ( auto& it : m_RenderCells ){
		for ( int32 i = 0; i < 6; ++i ){
			if ( CheckVisibleFace( it, i ) ){
				m_VisibleFace[it]	|= (1 << i);
			}
		}
	}

	m_Revision++;
	SetInvalidate( true );
}

//! 
bool UNaChunk::CheckVisibleFace( int32 index, int32 dir )
{
	const FIntVector	c_dirVec[6] = 
	{
		//! 前、左、奥、右
		FIntVector( -1, 0, 0 ), FIntVector( 0, -1, 0 ), FIntVector( 1, 0, 0 ), FIntVector( 0, 1, 0 ),
		//! 下、上
		FIntVector( 0, 0, -1 ), FIntVector( 0, 0, 1 )
	};
	const int32			c_dirHeight[4][4] =
	{
		{0, 2, 1, 3},
		{0, 1, 2, 3},
		{2, 0, 3, 1},
		{1, 0, 3, 2},
	};
	UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
	FNaBlockDataAsset*	srcBlock;
	FNaWorldBlockWork	work;
	FIntVector			vec;

	srcBlock = alib->FindBlockAsset( m_BlockID[index] );

	vec.X	= index & 0xF;
	vec.Y	= (index >> SHIFT_Y) & 0xF;
	vec.Z	= (index >> SHIFT_Z) & 0xF;
	vec		+= c_dirVec[dir];

	if ( GetBlock( vec.X, vec.Y, vec.Z, work ) ){
		if ( work.BlockID == 0 ){
			return true;
		}
		else {
			FNaBlockDataAsset*	dstBlock = alib->FindBlockAsset( work.BlockID );

			if ( srcBlock->IsOpaque && !dstBlock->IsOpaque ){
				return true;
			}
			else {
				switch ( dir ){
				case ENaMapBlockFace::Front:
				case ENaMapBlockFace::Back:
				case ENaMapBlockFace::Left:
				case ENaMapBlockFace::Right:
					if ( m_MetaData[index].Height[ c_dirHeight[dir][0] ] > work.MetaData.Height[ c_dirHeight[dir][1] ] && m_MetaData[index].Height[ c_dirHeight[dir][2] ] > work.MetaData.Height[ c_dirHeight[dir][3] ] ){
						return true;
					}
					break;
				case ENaMapBlockFace::Top:
					if ( ( m_MetaData[index].Height[0] & m_MetaData[index].Height[1] & m_MetaData[index].Height[2] & m_MetaData[index].Height[3] ) != 0xFF ){
						return true;
					}
					break;
				}
			}
		}
	}
	else {
		return true;
	}

	return false;
}

//
void UNaChunk::RecalcNeighborCells( int32 dir )
{
	int32	x,y,z;

	//! 描画面計算
	for ( auto& it : m_RenderCells ){
		x	= it & 0xF;
		y	= (it >> SHIFT_Y) & 0xF;
		z	= (it >> SHIFT_Z) & 0xF;
		
		switch ( dir ){
		case ENaMapBlockFace::Front:
			if ( x != 0 ){
				continue;
			}
			break;
		case ENaMapBlockFace::Back:
			if ( x != 0xF ){
				continue;
			}
			break;
		case ENaMapBlockFace::Left:
			if ( y != 0 ){
				continue;
			}
			break;
		case ENaMapBlockFace::Right:
			if ( y != 0xF ){
				continue;
			}
			break;
		case ENaMapBlockFace::Bottom:
			if ( z != 0 ){
				continue;
			}
			break;
		case ENaMapBlockFace::Top:
			if ( z != 0xF ){
				continue;
			}
			break;
		}

		if ( CheckVisibleFace( it, dir ) ){
			m_VisibleFace[it]	|= (1 << dir);
		}
		else {
			m_VisibleFace[it]	&= ~(1 << dir);
		}
	}
	
	SetInvalidate( true );
}

//
void UNaChunk::ConnectNeighborChunk( int32 dir, UNaChunk* neighbor )
{
	int32	rdir = GetReverseDir( dir );

	m_pNeighborChunk[dir]	= neighbor;
	RecalcNeighborCells( dir );

	neighbor->m_pNeighborChunk[rdir]	= this;
	neighbor->RecalcNeighborCells( rdir );
}

//
void UNaChunk::DisconnectNeighborChunk( int32 dir )
{
	int32	rdir = GetReverseDir( dir );

	if ( m_pNeighborChunk[dir] ){
		m_pNeighborChunk[dir]->m_pNeighborChunk[rdir]	= nullptr;
	}
	m_pNeighborChunk[dir]	= nullptr;
}

//! 
int32 UNaChunk::GetReverseDir( int32 dir ) const
{
	if ( dir & 0x4 ){
		return dir ^ 0x1;
	}
	else {
		return dir ^ 0x2;
	}
}

//! 
void UNaChunk::SetInvalidate( bool value )
{
//	if ( value && m_pRegion && GetPositionInWorld() == FIntVector::ZeroValue ){
//		GEngine->AddOnScreenDebugMessage( -1, 1.0f, FColor::Red, TEXT("AA") );
//	}
	m_bInvalidate	= value;
}
