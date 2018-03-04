// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaEntityManipulator.h"

#include "Entity/Character/NaEntityPlayer.h"
#include "Entity/Components/NaStateMachine.h"

// 操作ターゲット設定
void UNaEntityManipulator::BindTarget( UNaEntityCharacter* target )
{
	m_Target	= target;
}

// 更新
void UNaEntityManipulator::UpdateControl( float DeltaTime )
{
	OnControl( DeltaTime );
}

// 更新処理
void UNaEntityManipulator::OnControl( float DeltaTime )
{
	if ( m_Target ){
		UNaStateMachine*	sm = m_Target->GetStateMachine();

		switch ( sm->GetState() ){
		case UNaEntityCharacter::EState::EndTurn:
			{
				UNaTurnActionComponent*	tac = m_Target->GetTurnAction();
				tac->EndTurn();
			}
			break;
		}
	}
}

//! 
UWorld* UNaEntityManipulator::GetWorldContext() const
{
	return m_Target->GetWorldContext();
}
