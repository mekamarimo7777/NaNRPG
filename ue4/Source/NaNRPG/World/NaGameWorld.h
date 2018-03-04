// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaWorld.h"

#include "NaGameWorld.generated.h"

/**
 * ランタイムワールド
 */
UCLASS()
class NANRPG_API UNaGameWorld : public UNaWorld
{
	GENERATED_BODY()

public:
	
public:
	// コンストラクタ
	UNaGameWorld();

	//! 初期化
	virtual void	Initialize( UWorld* world ) override;
	//! 
	virtual void	Tick( float DeltaTime ) override;

	//! イベントマネージャ取得
	virtual UNaEventManager*	GetEventManager() override	{ return m_EventManager; }

protected:

public:
	
protected:
	//! イベントマネージャ
	UPROPERTY()
	UNaEventManager*	m_EventManager;

};
