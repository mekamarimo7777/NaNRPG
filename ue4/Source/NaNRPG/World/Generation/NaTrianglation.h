// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 三角形分割処理
 */
class FNaTrianglation
{
public:
	//! 内接円用定義
	struct FCircle
	{
		//! 中心点X
		int32	X;
		//! 中心点Y
		int32	Y;
		//! 半径
		float	Radius;
		//! 二乗半径（距離判定用）
		float	Radius2;
	};

	//! 辺
	struct FSegment
	{
		//! 
		FIntPoint	Start;
		//! 
		FIntPoint	End;

		FSegment(){}
		FSegment( FIntPoint& p0, FIntPoint& p1 )
		{
			Start	= p0;
			End		= p1;
		}

		//! 
		bool Contains( FIntPoint& p )
		{
			return Start == p || End == p;
		}

		//! 
		bool Equals( FSegment dst )
		{
			return (Start == dst.Start && End == dst.End) || (Start == dst.End && End == dst.Start);
		}
	};

	//! 三角形
	struct FTriangle
	{
		//! 頂点
		FIntPoint	Points[3];
		//! 辺
		FSegment	Segments[3];
		//! 外接円
		FCircle		Circle;

		FTriangle(){}
		FTriangle( FIntPoint& p0, FIntPoint& p1, FIntPoint& p2 )
		{
			Points[0]	= p0;
			Points[1]	= p1;
			Points[2]	= p2;

			Segments[0]	= FSegment( p0, p1 );
			Segments[1]	= FSegment( p1, p2 );
			Segments[2]	= FSegment( p2, p0 );

			GetCircumscribedCircle( p0, p1, p2, Circle );
		}

		//! 外接円計算
		void GetCircumscribedCircle( FIntPoint& p1, FIntPoint& p2, FIntPoint& p3, FCircle& circle )
		{
			int	c;

			int	xx1 = p1.X * p1.X;
			int	xx2 = p2.X * p2.X;
			int	xx3 = p3.X * p3.X;
			int	yy1 = p1.Y * p1.Y;
			int	yy2 = p2.Y * p2.Y;
			int	yy3 = p3.Y * p3.Y;

			int	t0 = xx2 - xx1 + yy2 - yy1;
			int	t1 = xx3 - xx1 + yy3 - yy1;

			c			= ((p2.X - p1.X) * (p3.Y - p1.Y) - (p2.Y - p1.Y) * (p3.X - p1.X)) * 2;
			if ( c == 0 ){
				circle.X	= 0;
				circle.Y	= 0;
			}
			else {
				circle.X	= ((p3.Y - p1.Y) * t0 + (p1.Y - p2.Y) * t1) / c;
				circle.Y	= ((p1.X - p3.X) * t0 + (p2.X - p1.X) * t1) / c;
			}

			t0	= p1.X - circle.X;
			t1	= p1.Y - circle.Y;
			circle.Radius2	= t0 * t0 + t1 * t1;
			circle.Radius	= (float)FMath::Sqrt( circle.Radius2 );
		}

		//! 
		bool IntersectPoint( FIntPoint& p )
		{
			int		dx,dy;
			float	dist;

			dx		= p.X - Circle.X;
			dy		= p.Y - Circle.Y;
			dist	= (dx * dx + dy * dy);

			return dist < Circle.Radius2;
		}

		//! 
		bool Equals( FTriangle& dst )
		{
			for ( int i = 0; i < 3; ++i ){
				if ( !dst.ContainsSegment( Segments[i] ) ){
					return false;
				}
			}
			return true;
		}

		//! 
		bool ContainsPoint( FIntPoint p )
		{
			return Points[0] == p || Points[1] == p || Points[2] == p;
		}

		//! 
		bool ContainsSegment( FSegment& seg )
		{
			return Segments[0].Equals( seg ) || Segments[1].Equals( seg ) || Segments[2].Equals( seg );
		}

		//! 
		FIntPoint GetAnotherPoint( FSegment seg )
		{
			if ( !seg.Contains( Points[0] )  ){
				return Points[0];
			}
			else if ( !seg.Contains( Points[1] )  ){
				return Points[1];
			}
			else {
				return Points[2];
			}
		}

		//! 
		void AppendAnotherSegments( FSegment seg, TArray<FSegment>& refVal )
		{
			if ( !Segments[0].Equals( seg ) ){
				refVal.Push( Segments[0] );
			}
			if ( !Segments[1].Equals( seg ) ){
				refVal.Push( Segments[1] );
			}
			if ( !Segments[2].Equals( seg ) ){
				refVal.Push( Segments[2] );
			}
		}
	};

public:
	//! 三角形分割
	static TArray<FTriangle>	Exec( TArray<FIntPoint>& points );

protected:
	//! 外部三角形取得
	static TSharedPtr<FNaTrianglation::FTriangle>	GetOuterTriangle( FIntRect& rect );
};
