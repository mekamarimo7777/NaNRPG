// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "MyPlayerController.h"



//
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	this->bShowMouseCursor		= true;
	this->bEnableClickEvents	= true;
}

//
void AMyPlayerController::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );
}
