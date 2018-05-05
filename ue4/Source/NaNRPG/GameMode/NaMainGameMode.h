// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"

#include "Actor/World/NaWorldActor.h"
#include "Actor/Camera/NaCameraActor.h"

#include "World/NaWorld.h"

#include "Utility/Components/NaStateMachine.h"

#include "NaMainGameMode.generated.h"

/**
 * メインゲームモード
 */
UCLASS()
class NANRPG_API ANaMainGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	//! ステート
	enum EState
	{
		Main,
	};

public:
	//! コンストラクタ
	ANaMainGameMode( const FObjectInitializer& ObjectInitializer );

	//! 開始処理
	virtual void	BeginPlay() override;
	//! 終了処理
	virtual void	EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//! 更新
	virtual void	Tick(float DeltaTime) override;

protected:
	//! メイン
	void	ProcMain( UNaStateMachine* sm, float DeltaTime );

	//! ワールド管理アクター生成
	void	CreateWorldActor();

public:
	//! カメラアクタークラス
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
	TSubclassOf<ANaCameraActor>		CameraActorClass;
	//! ワールド管理アクタークラス
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "World Actor" )
	TSubclassOf<ANaWorldActor>		WorldActorClass;

protected:
	//! 状態管理
	UPROPERTY()
	UNaStateMachine*	m_SM;

	//! ワールドアクター
	UPROPERTY()
	ANaWorldActor*		m_WorldActor;
	//! カメラ
	UPROPERTY()
	ANaCameraActor*		m_Camera;
};
