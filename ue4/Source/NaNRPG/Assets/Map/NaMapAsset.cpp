// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaMapAsset.h"

#include "Actor/Entity/NaActorBase.h"

#define	MAP_BLOCK_OFS(x, y, z)	(x + (y << FNaChunkData::SHIFT_Y) + (z << FNaChunkData::SHIFT_Z))

struct FNaMapNode
{
	bool				Horizontal;
	FIntRect			Rect;
	FIntRect			RoomRect;
	TArray<FIntRect>	Corridors;

	FNaMapNode()
	: Horizontal( false )
	{
	}

	bool IsDividable() const	{return Rect.Width() >= 12 && Rect.Height() >= 12;}
};

struct FNaMapGen
{
	TArray<FNaMapNode>	Nodes;
};

// 簡易ランダムマップ生成 //
void UNaMapAsset::CreateSimpleRandomMap( FIntVector size )
{
/*	FNaMapGen	gen;
	FNaMapNode	root;
	UNaMapPartsAsset*	partsAsset;

	m_MapSize		= size;
	partsAsset	= UNaMapPartsAsset::CreateDynamic( 0 );

	for ( int32 i = 0; i < 8; ++i ){
		gen.Nodes.Reset();

		// 区画分割 //
		root.Rect		= FIntRect( 0, 0, size.Y * 16 - 1, size.X * 16 - 1 );
		root.Horizontal	= FMath::Rand() & 0x1;
		root.Corridors.Reset();

		gen.Nodes.Add( root );
		DivideNode( &gen );

		// 部屋生成 //
		CreateRoom( &gen );

		// 通路生成 //
		CreateCorridor( &gen );
	
		// マップパーツへコンバート //
		{
			FNaMapPartsEntry	parts;
			int32	idx = 2;
			bool	bEnter = false;

			parts.Parts	= partsAsset;

			for ( auto& it : gen.Nodes ){
				parts.Position.X	= it.RoomRect.Min.Y;
				parts.Position.Y	= it.RoomRect.Min.X;
				parts.Position.Z	= i * -4 - 4;
				parts.Size.X		= it.RoomRect.Height() + 1;
				parts.Size.Y		= it.RoomRect.Width() + 1;
	//			parts.Size.Z		= bEnter ? 2 : 3;
				parts.Size.Z		= (it.Rect.Width() < 12 || it.Rect.Height() < 12) ? 4 : 2;
				m_Parts.Add( parts );

				for ( auto& it2 : it.Corridors ){
					parts.Position.X	= it2.Min.Y;
					parts.Position.Y	= it2.Min.X;
					parts.Position.Z	= i * -4 - 4;
					parts.Size.X		= it2.Height() + 1;
					parts.Size.Y		= it2.Width() + 1;
					parts.Size.Z		= 2;
					m_Parts.Add( parts );
				}

				parts.TestIdx	= idx++;
				bEnter	= true;
			}
		}
	}*/
}
//
void UNaMapAsset::DivideNode( FNaMapGen* gen )
{
	FNaMapNode	parent;
	FNaMapNode	child;
	int32		tmp;

	parent	= gen->Nodes.Pop();
//	if ( !parent.IsDividable() ){
//		gen->Nodes.Add( parent );
//		return;
//	}

	child.Horizontal	= !parent.Horizontal;
	child.Rect			= parent.Rect;

	if ( parent.Horizontal ){
		if ( parent.Rect.Width() < 12 ){
			gen->Nodes.Add( parent );
			return;
		}
		tmp	= FMath::RandRange( 6, parent.Rect.Width() - 6 );

		parent.Rect.Max.X	= parent.Rect.Min.X + tmp;
		child.Rect.Min.X	= parent.Rect.Max.X + 1;
	}
	else {
		if ( parent.Rect.Height() < 12 ){
			gen->Nodes.Add( parent );
			return;
		}
		tmp	= FMath::RandRange( 6, parent.Rect.Height() - 6 );

		parent.Rect.Max.Y	= parent.Rect.Min.Y + tmp;
		child.Rect.Min.Y	= parent.Rect.Max.Y + 1;
	}

	parent.Horizontal	^= true;
	if ( FMath::RandRange( 0, 1 ) ){
		gen->Nodes.Add( parent );
		gen->Nodes.Add( child );
	}
	else {
		gen->Nodes.Add( child );
		gen->Nodes.Add( parent );
	}

	DivideNode( gen );
}
//
void UNaMapAsset::CreateRoom( FNaMapGen* gen )
{
	for ( auto& it : gen->Nodes ){
		FIntPoint	size;

//		size.X	= FMath::RandRange( 2, it.Rect.Width() - 2 );
//		size.Y	= FMath::RandRange( 2, it.Rect.Height() - 2 );
		size.X	= it.Rect.Width() - 4;
		size.Y	= it.Rect.Height() - 4;

		it.RoomRect.Min.X	= it.Rect.Min.X + FMath::RandRange( 2, it.Rect.Width() - size.X - 2 );
		it.RoomRect.Min.Y	= it.Rect.Min.Y + FMath::RandRange( 2, it.Rect.Height() - size.Y - 2 );
		it.RoomRect.Max		= it.RoomRect.Min + size;
	}
}
//
void UNaMapAsset::CreateCorridor( FNaMapGen* gen )
{
	for ( int32 i = 0; i < gen->Nodes.Num() - 1; ++i ){
		ConnectRoom( &gen->Nodes[i], &gen->Nodes[i + 1] );
	}
}
//
void UNaMapAsset::ConnectRoom( FNaMapNode* src, FNaMapNode* dst )
{
	FIntRect	c0,c1,c2;

	if ( src->Horizontal ){
		// 上下接続 //
		if ( src->Rect.Min.Y < dst->Rect.Min.Y ){
			c0.Min.X	= FMath::RandRange( src->RoomRect.Min.X, src->RoomRect.Max.X );
			c0.Min.Y	= src->RoomRect.Max.Y;
			c0.Max.X	= c0.Min.X;
			c0.Max.Y	= src->Rect.Max.Y;
			src->Corridors.Add( c0 );

			c1.Min.X	= FMath::RandRange( dst->RoomRect.Min.X, dst->RoomRect.Max.X );
			c1.Min.Y	= dst->Rect.Min.Y;
			c1.Max.X	= c1.Min.X;
			c1.Max.Y	= dst->RoomRect.Min.Y;
			src->Corridors.Add( c1 );
		}
		else {
			c0.Min.X	= FMath::RandRange( dst->RoomRect.Min.X, dst->RoomRect.Max.X );
			c0.Min.Y	= dst->RoomRect.Max.Y;
			c0.Max.X	= c0.Min.X;
			c0.Max.Y	= dst->Rect.Max.Y;
			src->Corridors.Add( c0 );

			c1.Min.X	= FMath::RandRange( src->RoomRect.Min.X, src->RoomRect.Max.X );
			c1.Min.Y	= src->Rect.Min.Y;
			c1.Max.X	= c1.Min.X;
			c1.Max.Y	= src->RoomRect.Min.Y;
			src->Corridors.Add( c1 );
		}

		if ( c0.Min.X != c1.Min.X ){
			if ( c0.Min.X < c1.Min.X ){
				c2.Min.X	= c0.Min.X;
				c2.Max.X	= c1.Min.X;
			}
			else {
				c2.Min.X	= c1.Min.X;
				c2.Max.X	= c0.Min.X;
			}
			c2.Min.Y	= c2.Max.Y	= c0.Max.Y;
			src->Corridors.Add( c2 );
		}
	}
	else {
		// 左右接続 //
		if ( src->Rect.Min.X < dst->Rect.Min.X ){
			c0.Min.X	= src->RoomRect.Max.X;
			c0.Min.Y	= FMath::RandRange( src->RoomRect.Min.Y, src->RoomRect.Max.Y );
			c0.Max.X	= src->Rect.Max.X;
			c0.Max.Y	= c0.Min.Y;
			src->Corridors.Add( c0 );

			c1.Min.X	= dst->Rect.Min.X;
			c1.Min.Y	= FMath::RandRange( dst->RoomRect.Min.Y, dst->RoomRect.Max.Y );
			c1.Max.X	= dst->RoomRect.Min.X;
			c1.Max.Y	= c1.Min.Y;
			src->Corridors.Add( c1 );
		}
		else {
			c0.Min.X	= dst->RoomRect.Max.X;
			c0.Min.Y	= FMath::RandRange( dst->RoomRect.Min.Y, dst->RoomRect.Max.Y );
			c0.Max.X	= dst->Rect.Max.X;
			c0.Max.Y	= c0.Min.Y;
			src->Corridors.Add( c0 );

			c1.Min.X	= src->Rect.Min.X;
			c1.Min.Y	= FMath::RandRange( src->RoomRect.Min.Y, src->RoomRect.Max.Y );
			c1.Max.X	= src->RoomRect.Min.X;
			c1.Max.Y	= c1.Min.Y;
			src->Corridors.Add( c1 );
		}

		if ( c0.Min.Y != c1.Min.Y ){
			if ( c0.Min.Y < c1.Min.Y ){
				c2.Min.Y	= c0.Min.Y;
				c2.Max.Y	= c1.Min.Y;
			}
			else {
				c2.Min.Y	= c1.Min.Y;
				c2.Max.Y	= c0.Min.Y;
			}
			c2.Min.X	= c2.Max.X	= c0.Max.X;
			src->Corridors.Add( c2 );
		}
	}
}

//! 
void UNaMapAsset::ResizeMap( const FIntVector& size )
{
	m_MapSize	= size;
}

//! 
void UNaMapAsset::SetBlock( const FIntVector& pos, const FNaWorldBlockWork& work )
{
	FNaChunkData*	chunk;
	FIntVector		cpos,tpos;

	cpos.X	= pos.X >> 4;
	cpos.Y	= pos.Y >> 4;
	cpos.Z	= pos.Z >> 4;

	chunk	= m_Chunks.FindByPredicate( [&cpos]( const FNaChunkData& p )
	{
		return p.Position == cpos;
	});

	if ( !chunk ){
		int32	index;

		index	= m_Chunks.Add( FNaChunkData() );
		chunk	= &m_Chunks[index];
		chunk->Position	= cpos;
	}

	tpos	= pos;
	tpos.X	-= cpos.X << 4;
	tpos.Y	-= cpos.Y << 4;
	tpos.Z	-= cpos.Z << 4;
	int32	idx = MAP_BLOCK_OFS( tpos.X, tpos.Y, tpos.Z );

	chunk->ID[idx]		= work.BlockID;
	chunk->Param[idx]	= work.BlockParam;
	chunk->Meta[idx]	= work.MetaData.Value;
}
