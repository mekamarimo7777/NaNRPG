// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaCameraActor.h"

//====================================================================================================

//! �R���X�g���N�^
ANaCameraActor::ANaCameraActor( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer )
, OrbitalPitch( 35.0f )
, OrbitalRange( 100.0f )
, m_CurrentAngle( 0.0f )
, m_RequestAngle( 0.0f )
{
}

//! �J�n����
void ANaCameraActor::BeginPlay()
{
	Super::BeginPlay();
}

//! �X�V
void ANaCameraActor::Update( float DeltaSeconds )
{
	//! �p�x�ړ�
	if ( FMath::Abs( m_RequestAngle - m_CurrentAngle ) >= FLT_EPSILON ){
		float	diff;

		diff	= m_RequestAngle - m_CurrentAngle;
		if ( diff >= 180.0f ){
			diff	= diff - 360.0f;
		}
		else if ( diff <= -180.0f ){
			diff	= diff + 360.0f;
		}

		diff			= FMath::Clamp( diff, -10.0f, 10.0f );
		m_CurrentAngle	= FMath::Fmod( m_CurrentAngle + diff, 360.0f );
		ApplyCameraAngle();
	}

	//! �ʒu�ړ�
	if ( m_pTarget ){
		FVector	pos,dir;

		pos				= m_pTarget->GetActorLocation();
		m_CurrentPos	= FMath::Lerp( m_CurrentPos, pos, 0.1f );

		dir	= GetActorForwardVector();
		pos	= m_CurrentPos - dir * OrbitalRange;

		SetActorLocation( pos );
	}
}

//! �o�C���h��ݒ�
void ANaCameraActor::AttachTarget( AActor* target )
{
	if ( m_pTarget != target ){
		m_pTarget		= target;
		m_CurrentPos	= m_pTarget->GetActorLocation();

		ApplyCameraAngle();
	}
}

//! �A���O���ݒ�
void ANaCameraActor::SetAngle( float angle )
{
	m_RequestAngle	= FMath::Fmod( angle, 360.0f );
}

//====================================================================================================

//! �J�����p�x�ݒ�
void ANaCameraActor::ApplyCameraAngle()
{
	FVector	dir;

	dir	= FVector::ForwardVector;
	dir	= dir.RotateAngleAxis( OrbitalPitch, FVector::RightVector );
	dir	= dir.RotateAngleAxis( m_CurrentAngle, FVector::UpVector );

	SetActorRotation( FRotationMatrix::MakeFromX( dir ).Rotator() );
}
