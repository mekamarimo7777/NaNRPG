// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaCameraActor.h"

//
void ANaCameraActor::BeginPlay()
{
	Super::BeginPlay();

	OrbitalPos.Set( -100.0f, 0.0f, 75.0f );
	BirdEyePos.Set( -300.0f, 0.0f, 400.0f );
	OrbitalAngle	= 0.0f;

	m_UseBirdEye	= false;
}

//
void ANaCameraActor::Update( float DeltaSeconds )
{
	if ( m_pTarget ){
		FVector	tpos,pos,opos;

		pos		= GetActorLocation();
		tpos	= m_pTarget->GetActorLocation();

		if ( m_UseBirdEye ){
			opos	= BirdEyePos.RotateAngleAxis( OrbitalAngle, FVector::UpVector );
		}
		else {
			opos	= OrbitalPos.RotateAngleAxis( OrbitalAngle, FVector::UpVector );
		}
		
		pos	= tpos + opos;
		SetActorLocation( pos );

		SetActorRotation( FRotationMatrix::MakeFromX( tpos - pos ).Rotator() );
	}

	{
		UWorld*				world = GetWorld();
		APlayerController*	pc = world->GetFirstPlayerController();

		if ( pc->WasInputKeyJustPressed( EKeys::R ) ){
			OrbitalPos.Z	+= 10.0f;
		}
		else if ( pc->WasInputKeyJustPressed( EKeys::F ) ){
			OrbitalPos.Z	-= 10.0f;
		}

		if ( OrbitalAngleTo - OrbitalAngle > 180.0f ){
			OrbitalAngleTo	-= 360.0f;
		}
		else if ( OrbitalAngleTo - OrbitalAngle < -180.0f ){
			OrbitalAngleTo	+= 360.0f;
		}
		OrbitalAngle	+= (OrbitalAngleTo - OrbitalAngle) * 0.5f;

		if ( pc->WasInputKeyJustPressed( EKeys::B ) ){
			m_UseBirdEye	^= m_UseBirdEye;
		}
	}
}

//
void ANaCameraActor::AttachTarget( AActor* target )
{
	m_pTarget	= target;
}
