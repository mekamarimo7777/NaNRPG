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

	m_StateMachine->ChangeState( EState::Main );
}

//! Tick
void UNaMessageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );

	switch ( m_StateMachine->GetState() ){
	case EState::Main:
		ProcMain( m_StateMachine, InDeltaTime );
		break;
	}
}

//! キー入力
FReply UNaMessageWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	switch ( m_StateMachine->GetState() ){
	case EState::Main:
		ProcMain( m_StateMachine, 0.0f, &InKeyEvent );
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
void UNaMessageWidget::ProcMain( UNaStateMachine* sm, float DeltaTime, const FKeyEvent* KeyEvent )
{
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

	if ( !KeyEvent ){
		//! メイン
		switch ( sm->GetPhase() ){
		case Init:
			sm->AdvancePhase();
			break;

		case Wait:
			if ( !m_Text.IsEmpty() ){
				sm->AdvancePhase();
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
	else {
		const FKey	key = KeyEvent->GetKey();

		//! キーイベント
		switch ( sm->GetPhase() ){
		case Main:
			if ( key == EKeys::Enter ){
				sm->SetPhase( Decided );
			}
			break;
		}
	}
}
