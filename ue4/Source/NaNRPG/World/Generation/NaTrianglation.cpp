// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaTrianglation.h"

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//! 三角形分割
TArray<FNaTrianglation::FTriangle> FNaTrianglation::Exec( TArray<FIntPoint>& points )
{
	TArray<TSharedPtr<FTriangle> >	triangles;
	TSharedPtr<FTriangle>			outer_tri;

	//! 全域三角形を追加
	{
		FIntPoint	pMin( INT_MAX, INT_MAX );
		FIntPoint	pMax( INT_MIN, INT_MIN );
		FIntRect	rect;

		for ( auto& it : points ){
			pMin.X	= FMath::Min( pMin.X, it.X );
			pMin.Y	= FMath::Min( pMin.Y, it.Y );
			pMax.X	= FMath::Max( pMax.X, it.X );
			pMax.Y	= FMath::Max( pMax.Y, it.Y );
		}

		rect		= FIntRect( pMin.X, pMin.Y, pMax.X, pMax.Y );
		outer_tri	= GetOuterTriangle( rect );
		triangles.Add( outer_tri );
	}

	//! 
	for ( auto& p : points ){
		TArray<TSharedPtr<FTriangle> >	tri_stack;
		TArray<FSegment>	seg_stack;

		for ( auto& it : triangles ){
			if ( it->IntersectPoint( p ) ){
				tri_stack.Push( it );
			}
		}

		while ( tri_stack.Num() > 0 ){
			TSharedPtr<FTriangle>	tri = tri_stack.Pop();
			TSharedPtr<FTriangle>	t0,t1,t2;

			seg_stack.Push( tri->Segments[0] );
			seg_stack.Push( tri->Segments[1] );
			seg_stack.Push( tri->Segments[2] );

			triangles.Remove( tri );

			t0	= TSharedPtr<FTriangle>( new FTriangle( tri->Points[0], tri->Points[1], p ) );
			t1	= TSharedPtr<FTriangle>( new FTriangle( tri->Points[1], tri->Points[2], p ) );
			t2	= TSharedPtr<FTriangle>( new FTriangle( tri->Points[2], tri->Points[0], p ) );

			triangles.Add( t0 );
			triangles.Add( t1 );
			triangles.Add( t2 );
		}

		while ( seg_stack.Num() > 0 ){
			TSharedPtr<FTriangle>	tri_shared[2];
			FSegment	seg = seg_stack.Pop();
					
			{//! 
				int32	idx = 0;

				for ( auto& it : triangles ){
					if ( it->ContainsSegment( seg ) ){
						tri_shared[idx]	= it;
						if ( ++idx > 1 ){
							break;
						}
					}
				}

				if ( idx < 2 ){
					continue;
				}
				if ( tri_shared[0]->Equals( *tri_shared[1] ) ){
					triangles.Remove( tri_shared[0] );
					triangles.Remove( tri_shared[1] );
					continue;
				}
			}

			FIntPoint	sp, ep, p0, p1;

			sp	= seg.Start;
			ep	= seg.End;
			p0	= tri_shared[0]->GetAnotherPoint( seg );
			p1	= tri_shared[1]->GetAnotherPoint( seg );

			if ( tri_shared[0]->IntersectPoint( p1 ) ){
				TSharedPtr<FTriangle>	t0,t1;

				triangles.Remove( tri_shared[0] );
				triangles.Remove( tri_shared[1] );

				t0	= TSharedPtr<FTriangle>( new FTriangle( sp, p0, p1 ) );
				t1	= TSharedPtr<FTriangle>( new FTriangle( ep, p0, p1 ) );

				triangles.Add( t0 );
				triangles.Add( t1 );

				tri_shared[0]->AppendAnotherSegments( seg, seg_stack );
				tri_shared[1]->AppendAnotherSegments( seg, seg_stack );
			}
		}
	}

	//! 外部三角形削除
	triangles.RemoveAll( [&outer_tri]( TSharedPtr<FTriangle>& p )
	{
		return p->ContainsPoint( outer_tri->Points[0] ) || p->ContainsPoint( outer_tri->Points[1] ) || p->ContainsPoint( outer_tri->Points[2] );
	});

	TArray<FTriangle>	retVal;

	retVal.Reserve( triangles.Num() );
	for ( auto& it : triangles ){
		retVal.Add( *it );
	}

	return retVal;
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//! 
TSharedPtr<FNaTrianglation::FTriangle> FNaTrianglation::GetOuterTriangle( FIntRect& rect )
{
	int32	cx = rect.Min.X + rect.Width() / 2;
	int32	cy = rect.Min.Y + rect.Height() / 2;
	int32	r,r2,rr3;

	r	= (int)FMath::Sqrt( rect.Width() * rect.Width() + rect.Height() * rect.Height() ) / 2;
	r2	= r * 2;
	rr3	= (int)(r * FMath::Sqrt( 3 ));

	FIntPoint	p0,p1,p2;

	p0	= FIntPoint( cx - rr3, cy - r );
	p1	= FIntPoint( cx + rr3, cy - r );
	p2	= FIntPoint( cx, cy + r2 );

	return TSharedPtr<FNaTrianglation::FTriangle>( new FTriangle( p0, p1, p2 ) );
}
