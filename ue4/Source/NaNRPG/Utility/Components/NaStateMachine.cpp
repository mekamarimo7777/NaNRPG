// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaStateMachine.h"


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

//! 実行
void UNaStateMachine::Execute( float DeltaTime )
{
	FNaStateDelegate	func;

	while ( true ){
		if ( m_State != m_NextState ){
			ChangeState( m_NextState, m_NextStateParam, true );
		}

		if ( m_StateFunc.IsValidIndex( m_State ) ){
			func	= m_StateFunc[m_State];
			if ( func.IsBound() ){
				bool	finished;

				finished	= func.Execute( this, DeltaTime );
				if ( !finished ){
					continue;
				}
			}
		}

		break;
	}
}

//! 
void UNaStateMachine::ChangeState(int32 state, int32 param, bool immediate)
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

//! 
void UNaStateMachine::Update(float DeltaTime)
{
	if ( m_State != m_NextState ){
		ChangeState( m_NextState, m_NextStateParam, true );
	}
}
