// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "ProceduralMeshComponent.h"

#include "AtlasActor.generated.h"

// �O���[�o���}�b�v�A�N�^�[
UCLASS()
class NANRPG_API AAtlasActor : public AActor
{
	GENERATED_BODY()
	
public:
	// 
	AAtlasActor();

	// 
	virtual void BeginPlay() override;
	
	// 
	virtual void Tick( float DeltaSeconds ) override;

public:

protected:
};
