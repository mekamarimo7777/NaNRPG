// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPGEd.h"
#include "NaMapPartsActor.h"

//====================================================================================================

//
ANaMapPartsActor::ANaMapPartsActor( const FObjectInitializer& ObjectInitializer)
: Super( ObjectInitializer )
{
 	// 
	PrimaryActorTick.bCanEverTick = true;

	// ルートコンポーネント //
	m_ProcMesh		= CreateDefaultSubobject<UProceduralMeshComponent>( TEXT("ProceduralMesh") );
	m_ProcMesh->SetSimulatePhysics( false );
//	m_ProcMesh->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	RootComponent	= m_ProcMesh;
}

//! 
void ANaMapPartsActor::SetMapParts( UNaMapPartsAsset* parts )
{
	m_MapParts	= parts;
	UpdateMesh();
}

//!
void ANaMapPartsActor::UpdateMesh()
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

	UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
	int32	vertNum = 0;
	int32	indexNum = 0;

	GatherRenderCells();

	if ( m_RenderMaterial.Num() == 0 ){
		m_ProcMesh->ClearAllMeshSections();
		return;
	}

	for ( int32 i = 0; i < m_RenderMaterial.Num(); ++i ){
		FNaRenderMaterial*	rm = &m_RenderMaterial[i];
		int32	vert,idx;

		vertNum		= 0;
		indexNum	= 0;

		//! 描画面数
		for ( int32 j = 0; j < rm->CellIndices.Num(); ++j ){
			int32	fnum = m_RenderCells[ rm->CellIndices[j] ].Faces;
		
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
			FNaRenderCellWork*	cell = &m_RenderCells[ rm->CellIndices[j] ];
			FVector				offset( cell->X * 10.0f, cell->Y * 10.0f, cell->Z * 10.0f );

			FNaBlockDataAsset*	block = alib->FindBlockAsset( cell->Cell->BlockID );

			for ( int32 k = 0; k < 6; ++k ){
				if ( cell->Faces & (1 << k) ){
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

	//				u		= (work.BlockID & 0xF) / 16.0f;
	//				v		= (work.BlockID >> 4) / 16.0f;
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

		m_ProcMesh->SetMaterial( i, rm->Material );

		m_ProcMesh->ClearMeshSection( i );
//		m_ProcMesh->CreateMeshSection( i, rm->Vertices, rm->Indices, rm->Normals, rm->UVs, rm->Colors, rm->Tangents, false );
		m_ProcMesh->CreateMeshSection( i, rm->Vertices, rm->Indices, rm->Normals, rm->UVs, rm->Colors, rm->Tangents, true );
	}

	SetActorLocation( FVector( 0.0f, 0.0f, 0.0f ) );
}

//====================================================================================================

//! 
void ANaMapPartsActor::GatherRenderCells()
{
	UNaAssetLibrary*	alib = UNaAssetLibrary::Get();

	m_RenderCells.Reset();

	for ( int32 z = 0; z < m_MapParts->Size.Z; ++z ){
		for ( int32 y = 0; y < m_MapParts->Size.Y; ++y ){
			for ( int32 x = 0; x < m_MapParts->Size.X; ++x ){
				FNaRenderCellWork	work;

				work.Cell	= m_MapParts->GetCell( x, y, z );
				if ( work.Cell->BlockID > 0 ){
					work.X		= x;
					work.Y		= y;
					work.Z		= z;
					work.Faces	= 0xFF;
					m_RenderCells.Add( work );
				}
			}
		}
	}

	//! 描画面計算
	{
		const FIntVector	dirVec[6] = 
		{
			FIntVector( -1, 0, 0 ), FIntVector( 0, -1, 0 ), FIntVector( 1, 0, 0 ), FIntVector( 0, 1, 0 ),
			FIntVector( 0, 0, -1 ), FIntVector( 0, 0, 1 )
		};

		for ( int32 i = 0; i < m_RenderCells.Num(); ++i ){
			FNaRenderCellWork*	work = &m_RenderCells[i];
			FNaBlockDataAsset*	srcBlock = alib->FindBlockAsset( work->Cell->BlockID );

			work->Faces	= 0;

			for ( int32 j = 0; j < 4; ++j ){
				FIntVector	tmp( work->X, work->Y, work->Z );

				tmp	+= dirVec[j];
				if ( tmp.X >= 0 && tmp.Y >= 0 && tmp.X < m_MapParts->Size.X && tmp.Y < m_MapParts->Size.Y ){
					FNaMapPartsCell*	cell = m_MapParts->GetCell( tmp.X, tmp.Y, tmp.Z );

					if ( cell->BlockID == 0){
						work->Faces	|= (1 << j);
					}
					else {
						FNaBlockDataAsset*	dstBlock = alib->FindBlockAsset( cell->BlockID );

						if ( srcBlock->IsOpaque && !dstBlock->IsOpaque ){
							work->Faces	|= (1 << j);
						}
					}
				}
				else {
					work->Faces	|= (1 << j);
				}
			}

			for ( int32 j = 4; j < 6; ++j ){
				FIntVector	tmp( work->X, work->Y, work->Z );

				tmp	+= dirVec[j];
				if ( tmp.Z >= 0 && tmp.Z < m_MapParts->Size.Z ){
					FNaMapPartsCell*	cell = m_MapParts->GetCell( tmp.X, tmp.Y, tmp.Z );

					if ( cell->BlockID == 0){
						work->Faces	|= (1 << j);
					}
					else {
						FNaBlockDataAsset*	dstBlock = alib->FindBlockAsset( cell->BlockID );

						if ( srcBlock->IsOpaque && !dstBlock->IsOpaque ){
							work->Faces	|= (1 << j);
						}
					}
				}
				else {
					work->Faces	|= (1 << j);
				}
			}
		}
	}

	m_RenderMaterial.Reset();

	for ( int32 i = 0; i < m_RenderCells.Num(); ++i ){
		FNaRenderCellWork*	work = &m_RenderCells[i];
		FNaBlockDataAsset*	block;
		FNaRenderMaterial*	section;

		block	= alib->FindBlockAsset( work->Cell->BlockID );

		if ( block->MaterialID >= m_RenderMaterial.Num() ){
			m_RenderMaterial.SetNum( block->MaterialID + 1 );
		}

		if ( !m_RenderMaterial[block->MaterialID].MaterialAsset ){
			m_RenderMaterial[block->MaterialID].MaterialAsset	= alib->FindBlockMaterialAsset( block->MaterialID );

			TAssetPtr<UMaterialInterface>	aptr( m_RenderMaterial[block->MaterialID].MaterialAsset->Material );

			m_RenderMaterial[block->MaterialID].Material	= aptr.LoadSynchronous();
		}

		section	= &m_RenderMaterial[block->MaterialID];

		section->CellIndices.Add( i );
	}
}
