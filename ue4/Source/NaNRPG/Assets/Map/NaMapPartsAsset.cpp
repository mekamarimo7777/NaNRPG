// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaMapPartsAsset.h"

#include "Actor/Entity/NaActorBase.h"


// 簡易パーツ動的生成 //
UNaMapPartsAsset* UNaMapPartsAsset::CreateDynamic(int32 type)
{
	UNaMapPartsAsset*	retVal = NewObject<UNaMapPartsAsset>();

	retVal->CreateDynamicParts( type );

	return retVal;
}

void UNaMapPartsAsset::CreateDynamicParts(int32 type)
{
	switch ( type ){
	case 0:
		BoxOffsetX[0]	= BoxOffsetX[1]	= 0;
		BoxOffsetY[0]	= BoxOffsetY[1]	= 0;
		BoxOffsetZ[0]	= BoxOffsetZ[1]	= 0;
		Resize( FIntVector( 1, 1, 1 ) );
		GetCell( 0, 0, 0 )->BlockID	= 1;
		break;
	}
}

//
void UNaMapPartsAsset::Resize(FIntVector newSize)
{
	if ( Size != newSize ){
		TArray<FNaMapPartsCell>	oldCells;
		FIntVector	oldSize = Size;
		FIntVector	tmpSize;
		int32		tmp,tmp2;

		oldCells	= m_Cells;
		tmp			= newSize.X * newSize.Y * newSize.Z;
		m_Cells.SetNum( tmp );

		tmpSize.X	= FMath::Min( newSize.X, oldSize.X );
		tmpSize.Y	= FMath::Min( newSize.Y, oldSize.Y );
		tmpSize.Z	= FMath::Min( newSize.Z, oldSize.Z );

		for ( int32 z = 0; z < tmpSize.Z; ++z ){
			for ( int32 y = 0; y < tmpSize.Y; ++y ){
				for ( int32 x = 0; x < tmpSize.X; ++x ){
					tmp		= x + y * newSize.X + z * newSize.X * newSize.Y;
					tmp2	= x + y * oldSize.X + z * oldSize.X * oldSize.Y;
					m_Cells[tmp]	= oldCells[tmp2];
				}
			}
		}

		Size	= newSize;
	}
}

//
FNaMapPartsCell* UNaMapPartsAsset::GetWrapCell(int32 x, int32 y, int32 z)
{
	x	%= Size.X;
	y	%= Size.Y;
	z	%= Size.Z;
	return &m_Cells[x + y * Size.X + z * Size.X * Size.Y];
}
