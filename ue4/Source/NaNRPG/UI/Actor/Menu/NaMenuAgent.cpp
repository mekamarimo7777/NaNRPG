// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaMenuAgent.h"

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//! コンストラクタ
ANaMenuAgent::ANaMenuAgent( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer )
{
}

//! メニュー操作開始
void ANaMenuAgent::Start( int32 param )
{
	m_StateMachine->ChangeState( param );
}

//! 結果取得
ANaMenuAgent::EResult ANaMenuAgent::GetResult()
{
	return m_Result;
}

//! アイテム選択結果詳細
void ANaMenuAgent::GetItemResult()
{
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//! 更新
void ANaMenuAgent::OnTick( UNaStateMachine* sm, float DeltaTime )
{
	switch ( sm->GetState() ){
	case EState::Main:
		ProcMain( sm, DeltaTime );
		break;
	case EState::Inventory:
		ProcInventory( sm, DeltaTime );
		break;
	case EState::Equipment:
		ProcEquipment( sm, DeltaTime );
		break;
	}
}

//! メイン
void ANaMenuAgent::ProcMain( UNaStateMachine* sm, float DeltaTime )
{
	enum EPhase
	{
		//! 初期化
		Init,
		Start,
		//! メイン
		Main,
		//! 終了
		End,
	};

	switch ( sm->GetPhase() ){
	//! 初期化
	case Init:
		if ( !m_MenuWidget ){
			m_MenuWidget	= CreateWidget<UNaMenuWidget>( GetWorld(), MenuWidgetClass );
			m_MenuWidget->Open();
		}
		sm->AdvancePhase();
		break;
	case Start:
		m_MenuWidget->SetState( UNaMenuWidget::Main );
		sm->AdvancePhase();
		break;

	//! メイン
	case Main:
		break;

	//! 終了
	case End:
		sm->ChangeState( EState::None );
		break;
	}
}

//! インベントリ
void ANaMenuAgent::ProcInventory( UNaStateMachine* sm, float DeltaTime )
{
}

//! 装備
void ANaMenuAgent::ProcEquipment( UNaStateMachine* sm, float DeltaTime )
{
}
