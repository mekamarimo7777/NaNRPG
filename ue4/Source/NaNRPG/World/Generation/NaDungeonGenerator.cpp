// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaDungeonGenerator.h"

#include "NaTrianglation.h"

#include "World/NaChunk.h"

//! 部屋接続ワーク
struct FRoomConnect
{
	//! 
	int32	Src;
	//! 
	int32	Dest;
	//! 
	int32	Dist2;
	//! 
	bool	IsDummy;

	FRoomConnect(){}
	FRoomConnect( UNaDungeonGenerator::FRoom* src, UNaDungeonGenerator::FRoom* dst )
	{
		if ( src->ID < dst->ID ){
			Src		= src->ID;
			Dest	= dst->ID;
		}
		else {
			Src		= dst->ID;
			Dest	= src->ID;
		}

		Dist2	= (dst->Center - src->Center).SizeSquared();
		IsDummy	= false;
	}

	bool Equals( FRoomConnect& dst )
	{
		return Src == dst.Src && Dest == dst.Dest;
	}
};

//! UnionFind
struct FUnionFind
{
	//! ツリーデータ
	TArray<int32>	Data;

	//! 
	FUnionFind( int32 num = 0 )
	{
		Data.SetNum( num );

		for ( int i = 0; i < num; ++i ){
			Data[i]	= -1;
		}
	}

	int32 Root( int32 value )
	{
		if ( Data[value] < 0 ){
			return value;
		}
		else {
			return Data[value] = Root( Data[value] );
		}
	}

	void Merge( int32 v0, int32 v1 )
	{
		v0	= Root( v0 );
		v1	= Root( v1 );

		if ( v0 != v1 ){
			if ( Data[v1] < Data[v0] ){
				int	tmp = v0;
				v0	= v1;
				v1	= tmp;
			}

			Data[v0]	+= Data[v1];
			Data[v1]	= v0;
		}
	}

	bool Equals( int32 v0, int32 v1 )
	{
		return Root( v0 ) == Root( v1 );
	}
};

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//
UNaDungeonGenerator::UNaDungeonGenerator()
{

}

//! 生成
void UNaDungeonGenerator::Generate( const UNaMapAsset* asset )
{
	const FNaRandomMapParam*	param = asset->GetRandomMapParam();

	//! 部屋生成
	{
		int32	roomID = 0;

		for ( auto& it : param->RoomProcess ){

			m_Rooms.Reset();

			for ( int32 i = 0; i < it.RoomGen; ++i ){
				FRoom		room;
				FIntPoint	pos;

				pos	= GetRandomPointInCircle( it.Radius );

				room.ID		= roomID++;
				room.Pos.X	= pos.X;
				room.Pos.Y	= pos.Y;
				room.Size.X	= FMath::RandRange( it.MinRoomSize.X, it.MaxRoomSize.X );
				room.Size.Y	= FMath::RandRange( it.MinRoomSize.Y, it.MaxRoomSize.Y );

				room.IsMain	= room.Size.X >= it.MainRoomSize.X && room.Size.Y >= it.MainRoomSize.Y;
				room.IsSub	= false;

				room.WorkPos.X	= room.Pos.X;
				room.WorkPos.Y	= room.Pos.Y;

				m_Rooms.Add( room );
			}
		}

		//! 分散処理
		DiscreteRooms( m_Rooms );
	}

	//! 接続情報構築
	{
		TArray<FNaTrianglation::FTriangle>	triangles;

		//! 三角形分割
		{
			TArray<FIntPoint>	points;

			for ( auto& it : m_Rooms ){
				it.Center.X	= it.Pos.X + (it.Size.X >> 1);
				it.Center.Y	= it.Pos.Y + (it.Size.Y >> 1);

				if ( it.IsMain ){
					FIntPoint	p;

					p.X	= it.Center.X;
					p.Y	= it.Center.Y;
					points.Add( p );
				}
			}

			triangles	= FNaTrianglation::Exec( points );
		}

		{//! 接続マップ構築
			TArray<FRoomConnect>	connects;

			for ( auto& it : triangles ){
				FRoom*	rooms[3];
				int32	count = 0;
					
				for ( auto& it2 : m_Rooms ){
					if ( it2.IsMain ){
						for ( int i = 0; i < 3; ++i ){
							if ( it.Points[i] == it2.Center ){
								rooms[count]	= &it2;
								count++;
								break;
							}
						}

						if ( count >= 3 ){
							break;
						}
					}
				}

				FRoomConnect	connect;

				connect	= FRoomConnect( rooms[0], rooms[1] );
				if ( !connects.ContainsByPredicate( [&connect]( FRoomConnect& p ){ return p.Equals( connect ); } ) ){
					connects.Add( connect );
				}

				connect	= FRoomConnect( rooms[1], rooms[2] );
				if ( !connects.ContainsByPredicate( [&connect]( FRoomConnect& p ){ return p.Equals( connect ); } ) ){
					connects.Add( connect );
				}

				connect	= FRoomConnect( rooms[2], rooms[0] );
				if ( !connects.ContainsByPredicate( [&connect]( FRoomConnect& p ){ return p.Equals( connect ); } ) ){
					connects.Add( connect );
				}
			}

			{//! 最小全域ツリー構築
				TArray<FRoomConnect>	connects2;
				TArray<FRoomConnect>	connects3;
				FUnionFind	uf( m_Rooms.Num() );

				connects.StableSort( []( const FRoomConnect& lhs, const FRoomConnect& rhs )
				{
					return lhs.Dist2 < rhs.Dist2;
				});

				for ( auto& it : connects ){
					if ( !uf.Equals( it.Src, it.Dest ) ){
						connects2.Add( it );
						uf.Merge( it.Src, it.Dest );
					}
					else {
						connects3.Add( it );
					}
				}
					
				//! 無作為に復活
				for ( auto& it : connects3 ){
					if ( FMath::RandRange( 0, 8 ) == 0 ){
						connects2.Add( it );
					}
				}

				for ( auto& it : connects2 ){
					m_Rooms[it.Src].Connection.Add( it.Dest );
				}
			}
		}

		CreateCorridors();

		IncludeSubRooms();
	}

	//! チャンク生成
	{
		BuildChunkData();
	}
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//! 正規分布
FIntPoint UNaDungeonGenerator::GetRandomPointInCircle( float radius )
{
	FIntPoint	retVal;
	float		t = 2.0f * PI * FMath::FRand();
	float		u = FMath::FRand() + FMath::FRand();
	float		r;

	if ( u > 1.0f ){
		r	= 2 - u;
	}
	else {
		r	= u;
	}

	retVal.X	= FMath::RoundToInt( radius * r * FMath::Cos( t ) );
	retVal.Y	= FMath::RoundToInt( radius * r * FMath::Sin( t ) );

	return retVal;
}

//! 部屋分散
void UNaDungeonGenerator::DiscreteRooms( TArray<FRoom>& rooms, int32 maxLoop )
{
	bool	isComp = false;

	while ( --maxLoop >= 0 && !isComp ){
		isComp	= true;

		for ( int i = 0; i < m_Rooms.Num(); ++i ){
			FRoom&		room = m_Rooms[i];
			FIntRect	rect = FIntRect( (int32)room.WorkPos.X, (int32)room.WorkPos.Y, room.Pos.X + room.Size.X, room.Pos.Y + room.Size.Y );

			for ( int j = 0; j < m_Rooms.Num(); ++j ){
				if ( i == j ){
					continue;
				}

				FRoom&		room2 = m_Rooms[j];
				FIntRect	rect2 = FIntRect( (int32)room2.WorkPos.X, (int32)room2.WorkPos.Y, room2.Pos.X + room2.Size.X, room2.Pos.Y + room2.Size.Y );

				if ( rect.Min.X <= rect2.Max.X && rect2.Min.X <= rect.Max.X )
				if ( rect.Min.Y <= rect2.Max.Y && rect2.Min.Y <= rect.Max.Y ){
					FVector2D	vec = room.WorkPos - room2.WorkPos;

					if ( vec.IsZero() ){
						vec	= room.WorkPos;
					}
					vec.Normalize();

					room.WorkPos	+= vec;

					room.Pos.X	= (int32)room.WorkPos.X;
					room.Pos.Y	= (int32)room.WorkPos.Y;
					isComp	= false;
					break;
				}
			}
		}
	}
}

//! 通路生成
void UNaDungeonGenerator::CreateCorridors()
{
	m_Corridors.Reset();

	for ( auto& it : m_Rooms ){
		for ( auto& it2 : it.Connection ){
			FRoom&		dst = m_Rooms[it2];
			FCorridor	cor;

			int32	x0, x1;
			int32	y0, y1;

			x0	= FMath::Max( it.Pos.X, dst.Pos.X );
			x1	= FMath::Min( it.Pos.X + it.Size.X, dst.Pos.X + dst.Size.X );

			y0	= FMath::Max( it.Pos.Y, dst.Pos.Y );
			y1	= FMath::Min( it.Pos.Y + it.Size.Y, dst.Pos.Y + dst.Size.Y );
					
			if ( x1 - x0 >= 3 ){
				cor.Pos.X	= (x0 + x1) / 2;
				cor.Pos.Y	= y1;
				cor.Size.X	= 1;
				cor.Size.Y	= y0 - y1;
				m_Corridors.Add( cor );
			}
			else if ( y1 - y0 >= 3 ){
				cor.Pos.X	= x1;
				cor.Pos.Y	= (y0 + y1) / 2;
				cor.Size.X	= x0 - x1;
				cor.Size.Y	= 1;
				m_Corridors.Add( cor );
			}
			else {
				FRoom*	r0, *r1;

				if ( it.Center.X < dst.Center.X ){
					r0	= &it;
					r1	= &dst;
				}
				else {
					r0	= &dst;
					r1	= &it;
				}

				if ( FMath::Abs( x0 - x1 ) > FMath::Abs( y0 - y1 ) ){
					cor.Pos.X	= x1;
					cor.Pos.Y	= r0->Center.Y;
					cor.Size.X	= r1->Center.X - x1;
					cor.Size.Y	= 1;
					m_Corridors.Add( cor );

					cor.Pos.X	= r1->Center.X;
					cor.Size.X	= 1;
					if ( r0->Center.Y < r1->Center.Y ){
						cor.Pos.Y	= r0->Center.Y;
						cor.Size.Y	= r1->Pos.Y - cor.Pos.Y;
					}
					else {
						cor.Pos.Y	= r1->Pos.Y + r1->Size.Y;
						cor.Size.Y	= r0->Center.Y - cor.Pos.Y + 1;
					}
					m_Corridors.Add( cor );
				}
				else {
					cor.Pos.X	= r0->Center.X;
					cor.Size.X	= 1;
					if ( r0->Center.Y < r1->Center.Y ){
						cor.Pos.Y	= r0->Pos.Y + r0->Size.Y;
						cor.Size.Y	= r1->Center.Y - cor.Pos.Y;
					}
					else {
						cor.Pos.Y	= r1->Center.Y;
						cor.Size.Y	= r0->Pos.Y - cor.Pos.Y;
					}
					m_Corridors.Add( cor );

					cor.Pos.X	= r0->Center.X;
					cor.Size.X	= x0 - r0->Center.X;
					cor.Pos.Y	= r1->Center.Y;
					cor.Size.Y	= 1;
					m_Corridors.Add( cor );
				}
			}
		}
	}
}

//! サブ部屋追加
void UNaDungeonGenerator::IncludeSubRooms()
{
	FBox2D	r0,r1;

	for ( auto& it : m_Corridors ){
		r0.Min.X	= it.Pos.X;
		r0.Min.Y	= it.Pos.Y;
		r0.Max.X	= it.Pos.X + it.Size.X;
		r0.Max.Y	= it.Pos.Y + it.Size.Y;

		for ( auto& it2 : m_Rooms ){
			if ( it2.IsMain || it2.IsSub ){
				continue;
			}

			r1.Min.X	= it2.Pos.X;
			r1.Min.Y	= it2.Pos.Y;
			r1.Max.X	= it2.Pos.X + it2.Size.X;
			r1.Max.Y	= it2.Pos.Y + it2.Size.Y;

			if ( r0.Intersect( r1 ) ){
				it2.IsSub	= true;
			}
		}
	}
}

//! チャンクデータ構築
void UNaDungeonGenerator::BuildChunkData()
{
	UNaChunk*	chunk = nullptr;
	FIntVector	cpos;
	FIntVector	pos,tpos;

	//! 使用エリアを空気ブロックに変換
	{
		FNaWorldBlockWork	block;

		block.BlockID			= 2;
		block.MetaData.Value	= 0xFFFFFFFF;

		//! 
		for ( auto& it : m_Rooms ){
			if ( !it.IsMain && !it.IsSub ){
				continue;
			}

			for ( int32 x = 0; x < it.Size.X; ++x ){
				pos.X	= it.Pos.X + x + 32;
				
				if ( pos.X < 0 ){
					continue;
				}

				for ( int32 y = 0; y < it.Size.Y; ++y ){
					pos.Y	= it.Pos.Y + y + 32;
					
					if ( pos.Y < 0 ){
						continue;
					}

					for ( int32 z = 1; z < 2; ++z ){
						pos.Z	= z;

						cpos.X	= pos.X >> 4;
						cpos.Y	= pos.Y >> 4;
						cpos.Z	= pos.Z >> 4;
						if ( !chunk || cpos != chunk->GetPosition() ){
							chunk	= GetChunk( cpos );
						}

						tpos.X	= pos.X - (cpos.X << 4);
						tpos.Y	= pos.Y - (cpos.Y << 4);
						tpos.Z	= pos.Z - (cpos.Z << 4);

						chunk->SetBlock( tpos, block );
					}
				}
			}
		}

		block.BlockID			= 1;

		//! 
		for ( auto& it : m_Corridors ){
			for ( int32 x = 0; x < it.Size.X; ++x ){
				pos.X	= it.Pos.X + x + 32;
				
				if ( pos.X < 0 ){
					continue;
				}

				for ( int32 y = 0; y < it.Size.Y; ++y ){
					pos.Y	= it.Pos.Y + y + 32;
					
					if ( pos.Y < 0 ){
						continue;
					}

					for ( int32 z = 1; z < 2; ++z ){
						pos.Z	= z;

						cpos.X	= pos.X >> 4;
						cpos.Y	= pos.Y >> 4;
						cpos.Z	= pos.Z >> 4;
						if ( !chunk || cpos != chunk->GetPosition() ){
							chunk	= GetChunk( cpos );
						}

						tpos.X	= pos.X - (cpos.X << 4);
						tpos.Y	= pos.Y - (cpos.Y << 4);
						tpos.Z	= pos.Z - (cpos.Z << 4);

						chunk->SetBlock( tpos, block );
					}
				}
			}
		}
	}
}

//! チャンク生成or取得
UNaChunk* UNaDungeonGenerator::GetChunk( FIntVector cpos )
{
	UNaChunk*	chunk;

	chunk	= m_Chunks.FindRef( cpos );
	if ( !chunk ){
		chunk	= NewObject<UNaChunk>();
		chunk->Initialize( nullptr, cpos );
		m_Chunks.Add( cpos, chunk );
	}

	return chunk;
}
