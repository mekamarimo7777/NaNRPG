// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaStateMachine.h"


//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//!
UNaStateMachine::UNaStateMachine( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer )
, m_State( -1 )
, m_StateParam( 0 )
, m_StatePhase( 0 )
, m_NextState( -1 )
, m_NextStateParam( -1 )
{
	m_StateFunc.Reserve( 16 );
}

//! ステート登録
void UNaStateMachine::RegisterState( int32 state, FNaStateDelegate func )
{
	if ( m_StateFunc.Num() <= state ){
		m_StateFunc.SetNum( state + 1 );
	}
	m_StateFunc[state]	= func;
}

//! ステート解除
void UNaStateMachine::UnregisterState( int32 state )
{
	if ( m_StateFunc.IsValidIndex( state ) ){
		m_StateFunc[state].Unbind();
	}
}

//! 実行
void UNaStateMachine::Execute( float DeltaTime )
{
	FNaStateDelegate	func;

	m_Again	= true;

	while ( m_Again ){
		m_Again	= false;

		if ( m_State != m_NextState ){
			ChangeState( m_NextState, m_NextStateParam, true );
		}

		if ( m_StateFunc.IsValidIndex( m_State ) ){
			func	= m_StateFunc[m_State];
			if ( func.IsBound() ){
				func.Execute( this, DeltaTime );
			}
		}
	}
}

//! ステート変更
void UNaStateMachine::ChangeState( int32 state, int32 param, bool immediate )
{
	if ( immediate ){
		m_State			= state;
		m_StateParam	= param;
		m_StatePhase	= 0;
	}
	else {
		m_NextState			= state;
		m_NextStateParam	= param;
	}
}

//! フェーズ設定
void UNaStateMachine::SetPhase( int32 phase, bool again )
{
	m_StatePhase	= phase;
	m_Again			= again;
}

//! フェーズ進行
void UNaStateMachine::Advance( bool again )
{
	m_StatePhase++;
	m_Again	= again;
}
