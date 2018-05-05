// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"

#include "World/NaWorld.h"

#include "Entity/NaEntity.h"

#include "NaActorBase.generated.h"

UCLASS()
class NANRPG_API ANaActorBase : public AActor
{
	GENERATED_BODY()

public:
	enum EState
	{
		None,

		Waiting,
		Action,
		Attack,
		Damage,
		Death
	};
	
public:	
	// Sets default values for this actor's properties
	ANaActorBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
public:
	// 初期化
	void	Initialize( UNaWorld* world, UNaEntity* entity );

	//
	void	ChangeState(EState state, int32 param = 0, bool immediate = false);

	// 移動処理関連 //
	// 
	bool		SetWorldPosition( const FIntVector& pos, bool teleport = false );
	// 
	FIntVector	GetWorldPosition() const	{return m_pEntity->GetWorldPosition();}
	// 
//	bool		FindMovePosition( int32 dir, FIntVector& outPos );

	//
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool	IsMoving() const	{return m_bMoving;}
	//
	bool	IsWaiting() const	{return m_State == EState::Waiting;}

	void	Kill()	{m_bKill = true;}

	// 
	void			SetDirection( ENaDirection dir )	{m_pEntity->SetDirection( dir );}
	ENaDirection	GetDirection()						{return m_pEntity->GetDirection();}
//	int32	GetWaitTime() const						{return m_pEntity->GetWaitTime();}
//	void	ResetWaitTime()							{m_pEntity->ResetWaitTime();}
//	void	DecreaseWaitTime(int32 value)			{m_pEntity->DecreaseWaitTime( value );}

	//!
	UFUNCTION(BlueprintImplementableEvent, Category = "Actor")
	void	OnInitializeActor();

protected:
	//
	FVector	GetWorldLocation() const;

	//
	virtual void	OnInitialize()	{}

protected:
	// 基本情報 //
	UPROPERTY()
	UNaWorld*	m_pWorld;		// Naワールド
	UPROPERTY()
	UNaEntity*	m_pEntity;		// エンティティ

	//
	EState	m_State;
	int32	m_StateParam;
	int32	m_StateStep;

	// 移動
	FVector			m_DestLocation;
	FRotator		m_DestRotate;
	TArray<FVector>	m_DestList;
	float			m_FallSpeed;

	bool	m_bKill;
	bool	m_bMoving;
};
