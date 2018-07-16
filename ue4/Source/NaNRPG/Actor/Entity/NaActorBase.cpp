// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaActorBase.h"

#include "Actor/World/NaWorldActor.h"

// Sets default values
ANaActorBase::ANaActorBase()
: m_bKill( false )
{
	PrimaryActorTick.bCanEverTick	= true;
}

// Called when the game starts or when spawned
void ANaActorBase::BeginPlay()
{
	Super::BeginPlay();

	//! ステート管理
	m_SM	= NewObject<UNaStateMachine>();
}

// Called every frame
void ANaActorBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//! 状態更新
	m_SM->Execute( DeltaTime );

	{// 移動（仮）
		FVector		loc = GetActorLocation();
		FRotator	rot = GetActorRotation();

		if ( loc != m_DestLocation ){
			float	spd = m_Entity->GetSpeed() / 150.0f;

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
				m_DestRotate.Yaw	= int32( m_Entity->GetDirection() ) * 45.0f - 90.0f;
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

	//! 削除処理
	if ( m_bKill ){
		m_WorldActor->DestroyEntityActor( this );
	}
}

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//! 初期化
void ANaActorBase::Initialize( ANaWorldActor* worldActor, UNaEntity* entity )
{
	m_WorldActor	= worldActor;
	m_Entity		= entity;
	m_World			= m_Entity->GetNaWorld();

	OnInitialize();

	OnInitializeActor();
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

	chunk	= m_World->GetChunkFromWorld( wpos );
	if ( chunk ){
		FNaWorldBlockWork	block;
		FIntVector	pos = wpos - chunk->GetPositionInWorld() * 16;

		if ( chunk->GetBlock( pos, block ) ){
			loc.Z	+= block.MetaData.Height[0] / 255.0f * 10.0f;
		}
	}

	return loc;
}
