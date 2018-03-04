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

// �^�[���J�n����
void UNaTurnActionComponent::BeginTurn()
{
	m_IsEndTurn	= false;

	m_BeginTurnEvent.ExecuteIfBound();
}

// �^�[�����s
void UNaTurnActionComponent::ExecuteTurn( float DeltaTime )
{
	m_UpdateTurnEvent.ExecuteIfBound( DeltaTime );
}

// �^�[���I������
void UNaTurnActionComponent::EndTurn()
{
	m_IsEndTurn	= true;
}
