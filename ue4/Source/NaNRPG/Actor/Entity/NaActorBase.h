// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"

#include "World/NaWorld.h"

#include "Entity/NaEntity.h"

#include "Utility/Components/NaStateMachine.h"

#include "NaActorBase.generated.h"

class ANaWorldActor;

UCLASS()
class NANRPG_API ANaActorBase : public AActor
{
	GENERATED_BODY()

public:
	
public:	
	// Sets default values for this actor's properties
	ANaActorBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
public:
	//! 初期化
	void	Initialize( ANaWorldActor* worldActor, UNaEntity* entity );

	// 
	bool			SetWorldPosition( const FIntVector& pos, bool teleport = false );
	// 
	FIntVector		GetWorldPosition() const	{ return m_Entity->GetWorldPosition(); }
	//!
	void			SetDirection( ENaDirection dir )	{ m_Entity->SetDirection( dir ); }
	//! 
	ENaDirection	GetDirection() const				{ return m_Entity->GetDirection(); }

	//! アクション要求
	virtual void	RequestAction( FName action )	{}
	//! アクション中判定
	virtual bool	IsAction() const				{ return false; }

	//! 
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool	IsMoving() const	{ return m_bMoving; }

	//! キル要求
	void	Kill()	{ m_bKill = true; }

	//! 所属ワールド取得
	UNaWorld*		GetNaWorld() const	{ return m_World; }

	//! アクター初期化イベント
	UFUNCTION(BlueprintImplementableEvent, Category = "Actor")
	void	OnInitializeActor();

protected:
	//
	FVector	GetWorldLocation() const;

	//
	virtual void	OnInitialize()	{}

protected:
	//! ワールドアクター
	UPROPERTY(Transient)
	ANaWorldActor*	m_WorldActor;
	//! ゲームワールド
	UPROPERTY(Transient)
	UNaWorld*		m_World;
	//! エンティティ本体
	UPROPERTY(Transient)
	UNaEntity*		m_Entity;

	//! 状態管理
	UPROPERTY()
	UNaStateMachine*	m_SM;

	// 移動
	FVector			m_DestLocation;
	FRotator		m_DestRotate;
	TArray<FVector>	m_DestList;
	float			m_FallSpeed;

	//! 
	bool	m_bMoving;
	//! 
	bool	m_bKill;
};
