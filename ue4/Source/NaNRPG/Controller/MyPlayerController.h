// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NANRPG_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;
	
};
