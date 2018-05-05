// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaUIAgent.h"

//! �R���X�g���N�^
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

//! �X�V
void ANaUIAgent::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

	m_SM->Execute( DeltaSeconds );
}

//! ���s�҂�
bool ANaUIAgent::WaitForAction()
{
	return m_SM->GetState() == -1;
}
