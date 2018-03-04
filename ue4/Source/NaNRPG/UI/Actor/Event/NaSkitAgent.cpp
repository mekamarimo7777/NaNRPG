// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaSkitAgent.h"

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//! コンストラクタ
ANaSkitAgent::ANaSkitAgent( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer )
{
}

//! 開始
void ANaSkitAgent::Start( int32 param )
{
//	m_StateMachine->ChangeState( EState::Main );
}

//! メッセージ表示
void ANaSkitAgent::ShowMessage( FText message )
{
	m_Message	= message;

	m_StateMachine->ChangeState( EState::Message );
}

//! 
void ANaSkitAgent::HideMessage()
{
	m_StateMachine->ChangeState( EState::Selection );
}

//! 選択肢表示
void ANaSkitAgent::ShowSelection( const TArray<FText>& selections, int32 defaultIndex )
{
}

//! 選択インデックス取得
int32 ANaSkitAgent::GetSelectionIndex()
{
	return 0;
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//! 更新
void ANaSkitAgent::OnTick( UNaStateMachine* sm, float DeltaTime )
{
	switch ( sm->GetState() ){
	case EState::Message:
		ProcMessage( sm, DeltaTime );
		break;
	case EState::Selection:
		ProcSelection( sm, DeltaTime );
		break;
	}
}

//! メッセージ表示
void ANaSkitAgent::ProcMessage( UNaStateMachine* sm, float DeltaTime )
{
	enum EPhase
	{
		Init,
		Main,
		End,
	};

	switch ( sm->GetPhase() ){
	case Init:
		if ( !m_MessageWidget ){
			m_MessageWidget	= CreateWidget<UNaMessageWidget>( GetWorld(), MessageWidgetClass );
			m_MessageWidget->Open();
		}

		m_MessageWidget->MessageReached().AddLambda( [sm]()
		{
			sm->AdvancePhase();
		});

		m_MessageWidget->SetText( m_Message );
		m_MessageWidget->Transition( "Show" );

		sm->AdvancePhase();
		break;

	case Main:
		break;

	case End:
		m_MessageWidget->MessageReached().Clear();
		sm->ChangeState( EState::None );
		break;
	}
}

//! 選択肢表示
void ANaSkitAgent::ProcSelection( UNaStateMachine* sm, float DeltaTime )
{
}

/*
//! 
void ANaSkitAgent::ProcMain( UNaStateMachine* sm, float DeltaTime )
{
	enum EPhase
	{
		Init,
		Main,
		End,
	};
	
	switch ( sm->GetPhase() ){
	case Init:
		if ( PortalWidgetClass ){
			sm->AdvancePhase();
		}
		else {
			sm->SetPhase( End );
		}
		break;

	case Main:
		break;

	case End:
		sm->ChangeState( EState::None );
		break;
	}
}
*/
