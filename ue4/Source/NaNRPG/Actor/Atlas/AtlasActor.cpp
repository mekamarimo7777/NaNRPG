// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "AtlasActor.h"



AAtlasActor::AAtlasActor()
{
 	// 
	PrimaryActorTick.bCanEverTick = true;

	// ルートコンポーネント //
//	m_pProcMesh		= CreateDefaultSubobject<UProceduralMeshComponent>( TEXT("RootComponent") );
//	RootComponent	= m_pProcMesh;
}

// 
void AAtlasActor::BeginPlay()
{
	Super::BeginPlay();
}
	
// 
void AAtlasActor::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );
}
