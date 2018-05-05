// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaEntityManipulator.h"

#include "Entity/Character/NaEntityPlayer.h"

#include "Utility/Components/NaStateMachine.h"

// ����^�[�Q�b�g�ݒ�
void UNaEntityManipulator::BindTarget( UNaEntityCharacter* target )
{
	UnbindTarget();

	m_Target	= target;
}

// ����^�[�Q�b�g����
void UNaEntityManipulator::UnbindTarget()
{
	if ( m_Target ){
		m_Target	= nullptr;
	}
}

// �X�V
void UNaEntityManipulator::UpdateControl( UNaStateMachine* sm, float DeltaTime )
{
	OnControl( sm, DeltaTime );
}

// �X�V����
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

//! �^�[���G���h����
void UNaEntityManipulator::ProcEndTurn( UNaStateMachine* sm, float DeltaTime )
{
	UNaTurnActionComponent*	tac = m_Target->GetTurnAction();

	tac->EndTurn();
}
