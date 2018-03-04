// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaStateMachine.generated.h"

class UNaStateMachine;

//! �X�e�[�g�����f���Q�[�g
DECLARE_DELEGATE_RetVal_TwoParams( bool, FNaStateDelegate, UNaStateMachine*, float );

/**
 * �ėp�X�e�[�g�}�V��
 */
UCLASS()
class NANRPG_API UNaStateMachine : public UObject
{
	GENERATED_BODY()
	
public:
	//! �R���X�g���N�^
	UNaStateMachine( const FObjectInitializer& ObjectInitializer );

	//! �X�e�[�g�o�^
	void	RegisterState( int32 state, FNaStateDelegate func );

	//! ���s
	void	Execute( float DeltaTime );


	//! �X�e�[�g�ύX
	void	ChangeState(int32 state, int32 param = 0, bool immediate = false);
	//! �X�e�[�g�擾
	int32	GetState() const		{ return m_State; }
	//! �X�e�[�g�p�����[�^�擾
	int32	GetParam()				{ return m_StateParam; }
	//! 
//	void	PushState(int32 state);
	//! 
//	void	PopState(int32 state);

	//! 
	void	SetPhase(int32 phase)	{ m_StatePhase = phase; }
	//! 
	int32	GetPhase() const		{ return m_StatePhase; }
	//! 
	void	AdvancePhase()			{ m_StatePhase++; }

	//! �X�V
	void	Update( float DeltaTime );

protected:

public:
	
protected:
	//! �X�e�[�g
	TArray<FNaStateDelegate>	m_StateFunc;


	int32	m_State;
	int32	m_StateParam;
	int32	m_StatePhase;

	int32	m_NextState;
	int32	m_NextStateParam;

	bool	m_Continue;
};
