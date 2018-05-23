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
	m_SM->RegisterState( EState::Message, this, &ANaSkitAgent::ProcMessage );
	m_SM->RegisterState( EState::Selection, this, &ANaSkitAgent::ProcSelection );
}

//! メッセージ表示
void ANaSkitAgent::ShowMessage( FText message )
{
	m_Message	= message;

	m_SM->ChangeState( EState::Message );
}

//! メッセージ消去
void ANaSkitAgent::HideMessage()
{
	if ( m_MessageWidget ){
		m_MessageWidget->Transition( "Hide" );
	}
}

//! 選択肢表示
void ANaSkitAgent::ShowSelection( const TArray<FText>& selections, int32 defaultIndex )
{
	m_SM->ChangeState( EState::Selection );
}

//! 選択インデックス取得
int32 ANaSkitAgent::GetSelectionIndex()
{
	return 0;
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
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
			sm->Advance();
		});

		m_MessageWidget->SetText( m_Message );
		m_MessageWidget->Transition( "Show" );

		sm->Advance();
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
