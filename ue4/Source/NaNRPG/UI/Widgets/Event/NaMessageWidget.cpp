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
		m_SM->RegisterState( EState::Hide, this, &UNaMessageWidget::ProcHide );
		m_SM->RegisterState( EState::Show, this, &UNaMessageWidget::ProcShow );
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

	Transition( "Hide" );
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
	case EState::Show:
		ProcShowKeyDown( m_SM, &InKeyEvent );
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
//! 非表示
void UNaMessageWidget::ProcHide( UNaStateMachine* sm, float DeltaTime )
{
	//! 
	enum EPhase
	{
		//! 
		Init,
		//! 
		Transition,
		//! 
		Main,
		//! 
		End,
	};

	//! メイン
	switch ( sm->GetPhase() ){
	case Init:
		BeginTransition();
		sm->Advance();
		break;

	case Transition:
		if ( !HasTransTask() ){
			EndTransition();
			sm->Advance();
		}
		break;

	case Main:
		break;

	case End:
		break;
	}
}

//! メイン
void UNaMessageWidget::ProcShow( UNaStateMachine* sm, float DeltaTime )
{
	//! 
	enum EPhase
	{
		//! 
		Init,
		//! 
		Transition,
		//! 
		StartMain,
		Main,
		//! 
		Decided,
		//! 
		End,
	};

	//! メイン
	switch ( sm->GetPhase() ){
	case Init:
		BeginTransition();
		sm->Advance();
		break;

	case Transition:
		if ( !HasTransTask() ){
			EndTransition();
			sm->Advance();
		}
		break;

	case StartMain:
		if ( !m_Text.IsEmpty() ){
			sm->Advance();
		}
		break;
	case Main:
		break;

	case Decided:
		m_MessageReachedEvent.Broadcast();
		m_Text	= FText();
		sm->SetPhase( StartMain );
		break;

	case End:
		break;
	}
}
FReply UNaMessageWidget::ProcShowKeyDown( UNaStateMachine* sm, const FKeyEvent* KeyEvent )
{
	//! 
	enum EPhase
	{
		//! 
		Init,
		//! 
		Transition,
		//! 
		StartMain,
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

//! トランジションイベント
bool UNaMessageWidget::OnTransition( FName id )
{
	if ( id == "Hide" || id == "Exit" ){
		m_SM->ChangeState( EState::Hide );
	}
	else if ( id == "Show" ){
		m_SM->ChangeState( EState::Show );
	}

	return true;
}
