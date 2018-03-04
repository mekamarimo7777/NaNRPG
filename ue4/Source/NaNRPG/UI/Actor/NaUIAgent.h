// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "Utility/Components/NaStateMachine.h"

#include "NaUIAgent.generated.h"

UCLASS(abstract)
class NANRPG_API ANaUIAgent : public AActor
{
	GENERATED_BODY()
	
public:
	//! コンストラクタ
	ANaUIAgent( const FObjectInitializer& ObjectInitializer );

	//! 
	virtual void	BeginPlay() override;
	//! 更新
	virtual void	Tick( float DeltaSeconds ) override;

	//! 開始
	virtual void	Start( int32 param = 0 ){}
	//! 終了
	virtual void	Exit(){}

	//! 実行待ち
	virtual bool	WaitForAction();

protected:
	//! 更新
	virtual void	OnTick( UNaStateMachine* sm, float DeltaTime ){}

protected:
	//! 
	UPROPERTY()
	UNaStateMachine*	m_StateMachine;
	//! 
	bool				m_IsBusy;
};
