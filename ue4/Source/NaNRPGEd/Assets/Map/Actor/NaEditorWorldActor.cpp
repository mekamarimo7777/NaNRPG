// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPGEd.h"
#include "NaEditorWorldActor.h"

//====================================================================================================

//
ANaEditorWorldActor::ANaEditorWorldActor( const FObjectInitializer& ObjectInitializer)
: Super( ObjectInitializer )
{
 	// 
	PrimaryActorTick.bCanEverTick = true;
}

//! 
void ANaEditorWorldActor::SetNaWorld( UNaEditorWorld* naw )
{
	m_World	= naw;

	SetRenderSize( FIntVector(5, 5, 5) );
}

//! 
void ANaEditorWorldActor::SetRenderSize( FIntVector size )
{
	m_RenderSize	= size;

	for ( auto& it : m_ChunkActors ){
		it->Destroy();
	}

	int32	count;

	count	= m_RenderSize.X * m_RenderSize.Y;
	m_ChunkActors.SetNum( count );

	for ( auto& it : m_ChunkActors ){
		it	= GetWorld()->SpawnActor<ANaMapChunkActor>();
		it->SetCollidable( true );
#if WITH_EDITOR
		it->SetFolderPath( "/MapChunks" );
#endif							
	}
}

//! 
void ANaEditorWorldActor::UpdateChunk( FIntVector chunkPos )
{
	UNaChunk*	chunk;

	chunk	= m_World->GetChunk( chunkPos );

	if ( chunk ){
		ANaMapChunkActor*	actor;

		actor	= GetChunkActor( chunkPos );

		if ( actor ){
			chunk->RecalcRenderCells();
			actor->SetChunk( chunk );
		}
	}
}

//! 
void ANaEditorWorldActor::UpdateInvalidateChunk()
{
	UpdateChunkActor();
}

//====================================================================================================

//! 
void ANaEditorWorldActor::UpdateChunkActor()
{
	FIntVector	chunkPos;

	chunkPos.Z	= m_CurrentPos.Z - (m_RenderSize.Z >> 1);

	for ( int32 iz = 0; iz < m_RenderSize.Z; ++iz ){

		chunkPos.Y	= m_CurrentPos.Y - (m_RenderSize.Y >> 1);

		for ( int32 iy = 0; iy < m_RenderSize.Y; ++iy ){

			chunkPos.X	= m_CurrentPos.X - (m_RenderSize.X >> 1);

			for ( int32 ix = 0; ix < m_RenderSize.X; ++ix ){
				UNaChunk*			chunk;
				ANaMapChunkActor*	actor;

				chunk	= m_World->GetChunk( chunkPos );
				if ( chunk ){
					actor	= GetChunkActor( chunkPos );

					if ( actor ){
						actor->SetChunk( chunk );
					}
				}

				chunkPos.X++;
			}

			chunkPos.Y++;
		}

		chunkPos.Z++;
	}
}

//! 
ANaMapChunkActor* ANaEditorWorldActor::GetChunkActor( const FIntVector& chunkPos )
{
	FIntVector	apos;
	int32		tmp;

	apos.X	= chunkPos.X % m_RenderSize.X;
	apos.Y	= chunkPos.Y % m_RenderSize.Y;
	apos.Z	= 0;
	if ( apos.X < 0 ){
		apos.X	+= m_RenderSize.X;
	}
	if ( apos.Y < 0 ){
		apos.Y	+= m_RenderSize.Y;
	}

	tmp	= apos.X + apos.Y * m_RenderSize.X;

	return m_ChunkActors[tmp];
}
