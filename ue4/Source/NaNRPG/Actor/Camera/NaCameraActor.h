// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraActor.h"

#include "NaCameraActor.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class NANRPG_API ANaCameraActor : public ACameraActor
{
	GENERATED_BODY()
	
public:
	//
	virtual void BeginPlay() override;

	// 
	virtual void Update( float DeltaSeconds );

public:
	//
	void	AttachTarget( AActor* target );
	//
	void	SetOrbitalAngle( float angle )	{OrbitalAngleTo = angle;}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraActor)
	FVector	OrbitalPos;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraActor)
	float	OrbitalAngle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraActor)
	float	OrbitalAngleTo;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraActor)
	FVector	BirdEyePos;

protected:
	UPROPERTY()
	AActor*	m_pTarget;

	bool	m_UseBirdEye;
};
