// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaCanvasWidget.h"

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//! ウィジェット初期化
bool UNaCanvasWidget::Initialize()
{
	if ( Super::Initialize() ){
		m_SM->RegisterState( EState::Hide, this, &UNaCanvasWidget::ProcHide );
		m_SM->RegisterState( EState::Show, this, &UNaCanvasWidget::ProcShow );
		return true;
	}
	else {
		return false;
	}
}

//!
void UNaCanvasWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Transition( "Hide" );
}

//! Tick
void UNaCanvasWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );
}

//! オーナードロー
void UNaCanvasWidget::NativePaint( FPaintContext & InContext ) const
{
	
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//! 非表示
void UNaCanvasWidget::ProcHide( UNaStateMachine* sm, float DeltaTime )
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
		End,
	};

	//! メイン
	switch ( sm->GetPhase() ){
	case Init:
		sm->Advance();
		break;

	case Wait:
		break;

	case Main:
		break;

	case End:
		break;
	}
}

//! メイン
void UNaCanvasWidget::ProcShow( UNaStateMachine* sm, float DeltaTime )
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
		break;

	case Main:
		break;

	case Decided:
		break;

	case End:
		break;
	}
}

//! トランジションイベント
bool UNaCanvasWidget::OnTransition( FName id )
{
	if ( id == "Hide" || id == "Exit" ){
		m_SM->ChangeState( EState::Hide );
	}
	else if ( id == "Show" ){
		m_SM->ChangeState( EState::Show );
	}

	return true;
}
