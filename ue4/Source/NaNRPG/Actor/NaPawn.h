// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "NaPawn.generated.h"

//! ダミーポーン
UCLASS()
class NANRPG_API ANaPawn : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANaPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
