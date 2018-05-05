// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaActorBase.h"


// Sets default values
ANaActorBase::ANaActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if ( SphereVisualAsset.Succeeded() ){
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}*/
}

// Called when the game starts or when spawned
void ANaActorBase::BeginPlay()
{
	Super::BeginPlay();

	m_bKill	= false;
}

// Called every frame
void ANaActorBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	{// move
		FVector		loc = GetActorLocation();
		FRotator	rot = GetActorRotation();

		if ( loc != m_DestLocation ){
			float	spd = m_pEntity->GetSpeed() / 150.0f;

			if ( loc.X < m_DestLocation.X ){
				loc.X	= FMath::Min( loc.X + spd, m_DestLocation.X );
			}
			else if ( loc.X > m_DestLocation.X ){
				loc.X	= FMath::Max( loc.X - spd, m_DestLocation.X );
			}
			if ( loc.Y < m_DestLocation.Y ){
				loc.Y	= FMath::Min( loc.Y + spd, m_DestLocation.Y );
			}
			else if ( loc.Y > m_DestLocation.Y ){
				loc.Y	= FMath::Max( loc.Y - spd, m_DestLocation.Y );
			}
			if ( loc.Z < m_DestLocation.Z ){
				loc.Z	= FMath::Min( loc.Z + spd, m_DestLocation.Z );
			}
			else if ( loc.Z > m_DestLocation.Z ){
				loc.Z	= FMath::Max( loc.Z - m_FallSpeed, m_DestLocation.Z );
				m_FallSpeed	+= 0.1f;
			}

			SetActorLocation( loc, false );
		}

		if ( loc == m_DestLocation ){
			if ( m_DestList.Num() ){
				FVector	vec;

				m_DestLocation	= m_DestList[0];

				vec		= m_DestLocation - loc;
				if ( vec.X != 0.0f || vec.Y != 0.0f ){
					vec.Z	= 0.0f;
					vec.Normalize();
					m_DestRotate	= FRotationMatrix::MakeFromX( vec ).Rotator();
				}

				m_FallSpeed		= 0.0f;
				m_DestList.RemoveAt( 0 );
			}
			else {
				m_DestRotate.Yaw	= int32( m_pEntity->GetDirection() ) * 45.0f - 90.0f;
				if ( rot.Equals( m_DestRotate, 0.1f ) ){
					m_bMoving	= false;
				}
			}
		}

		if ( rot != m_DestRotate ){
			rot	= FMath::RInterpTo( rot, m_DestRotate, DeltaTime, 50.0f ); 
			SetActorRotation( rot );
		}
	}
}

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//
void ANaActorBase::Initialize( UNaWorld* world, UNaEntity* entity )
{
	m_pWorld	= world;
	m_pEntity	= entity;

	OnInitialize();

	OnInitializeActor();
}

//
void ANaActorBase::ChangeState(EState state, int32 param, bool immediate)
{
	m_State			= state;
	m_StateParam	= param;
	m_StateStep		= 0;
}

// 座標設定 //
bool ANaActorBase::SetWorldPosition( const FIntVector& pos, bool teleport )
{
	if ( teleport ){
		m_DestLocation	= GetWorldLocation();
		SetActorLocation( m_DestLocation, false );
	}
	else {
		m_DestList.Add( GetWorldLocation() );
	}
	m_bMoving	= true;

	return true;
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//
FVector	ANaActorBase::GetWorldLocation() const
{
	UNaChunk*	chunk;
	FIntVector	wpos = GetWorldPosition();
	FVector		loc;

	loc.X	= wpos.X * 10.0f + 5.0f;
	loc.Y	= wpos.Y * 10.0f + 5.0f;
	loc.Z	= wpos.Z * 10.0f;

	chunk	= m_pWorld->GetChunkFromWorld( wpos );
	if ( chunk ){
		FNaWorldBlockWork	block;
		FIntVector	pos = wpos - chunk->GetPositionInWorld() * 16;

		if ( chunk->GetBlock( pos, block ) ){
			loc.Z	+= block.MetaData.Height[0] / 255.0f * 10.0f;
		}
	}

	return loc;
}
