// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaMapChunkActor.h"

#include "NaWorldActor.h"

#include "Assets/NaAssetLibrary.h"

ANaMapChunkActor::ANaMapChunkActor( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer )
{
 	// 
	PrimaryActorTick.bCanEverTick = true;

	// ルートコンポーネント //
	//m_ProcMesh		= CreateDefaultSubobject<UProceduralMeshComponent>( TEXT("ProcMesh") );
	//RootComponent	= m_ProcMesh;

	RootComponent	= CreateDefaultSubobject<USceneComponent>( TEXT("SceneComp0") );

	m_Collidable	= false;
	m_WorldActor	= nullptr;
}

// 
void ANaMapChunkActor::BeginPlay()
{
	Super::BeginPlay();
}
	
// 
void ANaMapChunkActor::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

	for ( auto& it : m_Layer ){
		if ( it.Chunk && (it.Chunk->IsInvalidate() || it.Invalidate) ){
			UpdateChunkMesh( it.ID );
		}
	}
}

//
void ANaMapChunkActor::SetChunk( UNaChunk* chunk )
{
	int32	layer;

	layer	= chunk->GetPosition().Z;

	if ( !m_Layer.IsValidIndex(layer) ){
		m_Layer.SetNum( layer + 1 );
	}

	if ( m_Layer[layer].Chunk != chunk ){
		FVector	loc;

		m_Layer[layer].ID			= layer;
		m_Layer[layer].Chunk		= chunk;
		m_Layer[layer].Invalidate	= true;
			
		loc.X	= chunk->GetPositionInWorld().X * 160.0f;
		loc.Y	= chunk->GetPositionInWorld().Y * 160.0f;
		loc.Z	= 0.0f;
		SetActorLocation( loc );
	}
}

//!
void ANaMapChunkActor::UpdateChunkMesh( int32 layer )
{
	FVector	c_Vertex[8] = 
	{
		FVector( 1.0f, 0.0f, 1.0f ), FVector( 1.0f, 1.0f, 1.0f ), FVector( 0.0f, 0.0f, 1.0f ), FVector( 0.0f, 1.0f, 1.0f ),
		FVector( 1.0f, 0.0f, 0.0f ), FVector( 1.0f, 1.0f, 0.0f ), FVector( 0.0f, 0.0f, 0.0f ), FVector( 0.0f, 1.0f, 0.0f ),
	};
	const FVector	c_Normals[6] = 
	{
		FVector(-1.0f, 0.0f, 0.0f ), FVector( 0.0f,-1.0f, 0.0f ), 
		FVector( 1.0f, 0.0f, 0.0f ), FVector( 0.0f, 1.0f, 0.0f ), 
		FVector( 0.0f, 0.0f,-1.0f ), FVector( 0.0f, 0.0f, 1.0f ), 
	};
	const int32		c_VertexIdx[6][4] = 
	{
		{ 2, 3, 6, 7 },	// front
		{ 0, 2, 4, 6 },	// left
		{ 1, 0, 5, 4 },	// back
		{ 3, 1, 7, 5 },	// right
		{ 6, 7, 4, 5 },	// bottom
		{ 0, 1, 2, 3 },	// top
	};

	FNaChunkRenderLayer*	pLayer = &m_Layer[layer];
	const TArray<int16>&	cells = pLayer->Chunk->GetRenderCells();

	GatherRenderCells( layer );

	if ( cells.Num() == 0 ){
		if ( pLayer->ProcMesh ){
			pLayer->ProcMesh->ClearAllMeshSections();
		}
	}
	else {
		UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
		int32	vertNum = 0;
		int32	indexNum = 0;

		if ( !pLayer->ProcMesh ){
			UProceduralMeshComponent*	pmc;

			pmc	= NewObject<UProceduralMeshComponent>( this );
			pmc->RegisterComponentWithWorld( GetWorld() );
			pmc->AttachToComponent( RootComponent, FAttachmentTransformRules::KeepRelativeTransform );
			pmc->SetRelativeLocation( FVector( 0.0f, 0.0f, layer * 160.0f ) );

			pLayer->ProcMesh	= pmc;
		}

		for ( int32 i = 0; i < m_RenderMaterial.Num(); ++i ){
			FNaRenderMaterial*	rm = &m_RenderMaterial[i];
			FNaWorldBlockWork	work;
			int32	vert,idx;

			vertNum		= 0;
			indexNum	= 0;

			//! 描画面数
			for ( int32 j = 0; j < rm->CellIndices.Num(); ++j ){
				pLayer->Chunk->GetBlock( rm->CellIndices[j], work );

				int32	fnum = work.VisibleFace;
		
				fnum	= (fnum & 0x55) + (fnum >> 1 & 0x55);
				fnum	= (fnum & 0x33) + (fnum >> 2 & 0x33);
				fnum	= (fnum & 0x0f) + (fnum >> 4 & 0x0f);
				vertNum	+= fnum << 2;
			}
		
			rm->Vertices.SetNum( vertNum );
			rm->Normals.SetNum( vertNum );
			rm->UVs.SetNum( vertNum );
			rm->Colors.SetNum( vertNum );

			indexNum	= vertNum * 3 >> 1;
			rm->Indices.SetNum( indexNum );

			vert	= 0;
			idx		= 0;

			for ( int32 j = 0; j < rm->CellIndices.Num(); ++j ){
				int32	tmp,x,y,z;

				pLayer->Chunk->GetBlock( rm->CellIndices[j], work );

				tmp	= rm->CellIndices[j];
				x	= tmp & 0xF;
				y	= (tmp >> 4) & 0xF;
				z	= (tmp >> 8) & 0xF;

				FVector				offset( x * 10.0f, y * 10.0f, z * 10.0f );
				FNaBlockDataAsset*	block = alib->FindBlockAsset( work.BlockID );

				c_Vertex[0].Z	= work.MetaData.Height[2] / 255.0f;
				c_Vertex[1].Z	= work.MetaData.Height[3] / 255.0f;
				c_Vertex[2].Z	= work.MetaData.Height[0] / 255.0f;
				c_Vertex[3].Z	= work.MetaData.Height[1] / 255.0f;

				for ( int32 k = 0; k < 6; ++k ){
					if ( work.VisibleFace & (1 << k) ){
						rm->Vertices[vert + 0]	= c_Vertex[ c_VertexIdx[k][0] ] * 10.0f + offset;
						rm->Vertices[vert + 1]	= c_Vertex[ c_VertexIdx[k][1] ] * 10.0f + offset;
						rm->Vertices[vert + 2]	= c_Vertex[ c_VertexIdx[k][2] ] * 10.0f + offset;
						rm->Vertices[vert + 3]	= c_Vertex[ c_VertexIdx[k][3] ] * 10.0f + offset;

						rm->Normals[vert + 0]	= c_Normals[k];
						rm->Normals[vert + 1]	= c_Normals[k];
						rm->Normals[vert + 2]	= c_Normals[k];
						rm->Normals[vert + 3]	= c_Normals[k];

						float	uvadd,u,v;
						int32	bid = block->TileID[k];

						u		= (bid & 0xF) / 16.0f;
						v		= (bid >> 4) / 16.0f;
						uvadd	= 1.0f / 16.0f;

						rm->UVs[vert + 0].Set( u, v );
						rm->UVs[vert + 1].Set( u + uvadd, v );
						rm->UVs[vert + 2].Set( u, v + uvadd );
						rm->UVs[vert + 3].Set( u + uvadd, v + uvadd );

						rm->Indices[idx + 0]	= vert;
						rm->Indices[idx + 1]	= vert + 2;
						rm->Indices[idx + 2]	= vert + 1;
						rm->Indices[idx + 3]	= vert + 3;
						rm->Indices[idx + 4]	= vert + 1;
						rm->Indices[idx + 5]	= vert + 2;

						vert	+= 4;
						idx		+= 6;
					}
				}
			}

			pLayer->ProcMesh->SetMaterial( i, rm->Material );
			pLayer->ProcMesh->ClearMeshSection( i );
			pLayer->ProcMesh->CreateMeshSection( i, rm->Vertices, rm->Indices, rm->Normals, rm->UVs, rm->Colors, rm->Tangents, m_Collidable );
		}
	}

	pLayer->Chunk->SetInvalidate( false );
	pLayer->Invalidate	= false;
}

//====================================================================================================

//! 
void ANaMapChunkActor::GatherRenderCells( int32 layer )
{
	UNaAssetLibrary*		alib = UNaAssetLibrary::Get();
	FNaChunkRenderLayer*	pLayer = &m_Layer[layer];
	const TArray<int16>&	cells = pLayer->Chunk->GetRenderCells();

	//m_RenderMaterial.Reset();

	for ( int32 i = 0; i < cells.Num(); ++i ){
		FNaWorldBlockWork	work;
		FNaBlockDataAsset*	block;
		FNaRenderMaterial*	section;

		pLayer->Chunk->GetBlock( cells[i], work );

		block	= alib->FindBlockAsset( work.BlockID );

		if ( block->MaterialID >= m_RenderMaterial.Num() ){
			m_RenderMaterial.SetNum( block->MaterialID + 1 );
		}

		if ( !m_RenderMaterial[block->MaterialID].Material ){
			if ( m_WorldActor ){
				m_RenderMaterial[block->MaterialID].Material	= m_WorldActor->FindBlockMaterial( block->MaterialID );
			}
			else {
				FNaBlockMaterialAsset*			asset = alib->FindBlockMaterialAsset( block->MaterialID );
				TAssetPtr<UMaterialInterface>	aptr( asset->Material );

				m_RenderMaterial[block->MaterialID].Material	= aptr.LoadSynchronous();
			}
		}

		section	= &m_RenderMaterial[block->MaterialID];

		section->CellIndices.Add( cells[i] );
	}
}
