// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaPawn.h"


// Sets default values
ANaPawn::ANaPawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANaPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANaPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

