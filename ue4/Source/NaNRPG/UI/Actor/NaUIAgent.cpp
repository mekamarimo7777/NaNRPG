// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaUIAgent.h"

//! コンストラクタ
ANaUIAgent::ANaUIAgent( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer )
, m_IsBusy( false )
{
	m_SM	= CreateDefaultSubobject<UNaStateMachine>( "StateMachine", true );

	PrimaryActorTick.bCanEverTick = true;
}

//! 
void ANaUIAgent::BeginPlay()
{
	Super::BeginPlay();
}

//! 更新
void ANaUIAgent::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

	m_SM->Execute( DeltaSeconds );
}

//! 実行待ち
bool ANaUIAgent::WaitForAction()
{
	return m_SM->GetState() == -1;
}
