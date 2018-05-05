// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaEntityManipulator.h"

#include "Entity/Character/NaEntityPlayer.h"

#include "Utility/Components/NaStateMachine.h"

// 操作ターゲット設定
void UNaEntityManipulator::BindTarget( UNaEntityCharacter* target )
{
	UnbindTarget();

	m_Target	= target;
}

// 操作ターゲット解除
void UNaEntityManipulator::UnbindTarget()
{
	if ( m_Target ){
		m_Target	= nullptr;
	}
}

// 更新
void UNaEntityManipulator::UpdateControl( UNaStateMachine* sm, float DeltaTime )
{
	OnControl( sm, DeltaTime );
}

// 更新処理
void UNaEntityManipulator::OnControl( UNaStateMachine* sm, float DeltaTime )
{
	switch ( sm->GetState() ){
	case UNaEntityCharacter::EState::EndTurn:
		ProcEndTurn( sm, DeltaTime );
		break;
	}
}

//! 
UWorld* UNaEntityManipulator::GetWorldContext() const
{
	return m_Target->GetWorldContext();
}

//! ターンエンド処理
void UNaEntityManipulator::ProcEndTurn( UNaStateMachine* sm, float DeltaTime )
{
	UNaTurnActionComponent*	tac = m_Target->GetTurnAction();

	tac->EndTurn();
}
