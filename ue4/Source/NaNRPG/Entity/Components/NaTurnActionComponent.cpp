// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaTurnActionComponent.h"

#include "Entity/Controller/NaEntityManipulator.h"


//
UNaTurnActionComponent::UNaTurnActionComponent( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer )
, m_Speed( 100 )
, m_WaitTime( 0 )
{
}

// ターン開始処理
void UNaTurnActionComponent::BeginTurn()
{
	m_IsEndTurn	= false;

	m_BeginTurnEvent.ExecuteIfBound();
}

// ターン実行
void UNaTurnActionComponent::ExecuteTurn( float DeltaTime )
{
	m_UpdateTurnEvent.ExecuteIfBound( DeltaTime );
}

// ターン終了処理
void UNaTurnActionComponent::EndTurn()
{
	m_IsEndTurn	= true;
}
