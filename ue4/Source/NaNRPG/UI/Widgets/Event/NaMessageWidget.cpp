// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaMessageWidget.h"

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//! ウィジェット初期化
bool UNaMessageWidget::Initialize()
{
	if ( Super::Initialize() ){
		m_SM->RegisterState( EState::Main, this, &UNaMessageWidget::ProcMain );
		return true;
	}
	else {
		return false;
	}
}

//!
void UNaMessageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_SM->ChangeState( EState::Main );
}

//! Tick
void UNaMessageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );
}

//! キー入力
FReply UNaMessageWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	switch ( m_SM->GetState() ){
	case EState::Main:
		ProcMainKeyDown( m_SM, &InKeyEvent );
		break;
	}

	return FReply::Unhandled();
}

//! 
void UNaMessageWidget::SetText( FText text )
{
	m_Text	= text;

	SetKeyboardFocus();
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//! 
void UNaMessageWidget::ProcMain( UNaStateMachine* sm, float DeltaTime )
{
	//! 
	enum EPhase
	{
		//! 
		Init,
		//! 
		Wait,
		//! 
		Main,
		//! 
		Decided,
		//! 
		End,
	};

	//! メイン
	switch ( sm->GetPhase() ){
	case Init:
		sm->Advance();
		break;

	case Wait:
		if ( !m_Text.IsEmpty() ){
			sm->Advance();
		}
		break;

	case Main:
		break;

	case Decided:
		m_MessageReachedEvent.Broadcast();
		m_Text	= FText();
		sm->SetPhase( Wait );
		break;

	case End:
		break;
	}
}
FReply UNaMessageWidget::ProcMainKeyDown( UNaStateMachine* sm, const FKeyEvent* KeyEvent )
{
	//! 
	enum EPhase
	{
		//! 
		Init,
		//! 
		Wait,
		//! 
		Main,
		//! 
		Decided,
		//! 
		End,
	};
	const FKey	key = KeyEvent->GetKey();

	//! キーイベント
	switch ( sm->GetPhase() ){
	case Main:
		if ( key == EKeys::Enter ){
			sm->SetPhase( Decided );
		}
		break;
	}

	return FReply::Handled();
}
