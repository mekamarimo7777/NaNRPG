// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"

#include "Actor/World/NaWorldActor.h"
#include "Actor/Camera/NaCameraActor.h"

#include "GameModeAtlas.generated.h"

/**
 * グローバルマップゲームモード
 */
UCLASS()
class NANRPG_API AGameModeAtlas : public AGameMode
{
	GENERATED_BODY()

public:
	// コンストラクタ //
	AGameModeAtlas(const FObjectInitializer& ObjectInitializer);

	// 
	virtual void	BeginPlay() override;

	// 
	virtual void	Tick(float DeltaTime) override;

public:
	//
	void		ChangeState(int32 state, int32 param = 0, bool immediate = false);

protected:
	//
	void	ProcMain(float DeltaTime);

public:

protected:
	//
	int32	m_State;
	int32	m_StateParam;
	int32	m_StateStep;
};
