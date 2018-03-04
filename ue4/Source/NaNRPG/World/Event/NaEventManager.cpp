// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaEventManager.h"

#include "Singleton/NaGameInstance.h"

#include "Entity/Components/NaStateMachine.h"

#include "UI/Actor/Event/NaSkitAgent.h"

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//
UNaEventManager::UNaEventManager()
{
	m_StateMachine	= NewObject<UNaStateMachine>();
}

//！初期化
void UNaEventManager::Initialize( UNaWorld* world )
{
	m_World	= world;
}

//! 更新
void UNaEventManager::Tick( float DeltaTime )
{
	m_StateMachine->Update( DeltaTime );

	switch ( m_StateMachine->GetState() ){
	case EState::Playing:
		ProcEvent( m_StateMachine );
		break;
	case EState::Message:
		ProcMessage( m_StateMachine );
		break;
	case EState::Selection:
		ProcSelection( m_StateMachine );
		break;
	}
}

//! 実行要求
void UNaEventManager::PlayEvent( const UNaEventAsset* evt, int32 sheet )
{
	if ( !evt ){
		return;
	}

	m_Event			= evt;
	m_CurrentSheet	= sheet;
	m_PC			= -1;

	m_StateMachine->ChangeState( EState::Playing );
}

//! 実行中判定
bool UNaEventManager::IsPlaying() const
{
	return m_StateMachine->GetState() != EState::Idle;
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//! イベント実行メイン
void UNaEventManager::ProcEvent( UNaStateMachine* sm )
{
	enum EPhase
	{
		//! 初期化
		Init,
		//! メイン処理
		Main,
		//! 終了
		End
	};
	UNaGameInstance*	gi = UNaGameInstance::Get( m_World->GetWorldContext() );

	switch ( sm->GetPhase() ){
	//! 初期化
	case Init:
		{
			const FNaEventSheet*	sheet = &m_Event->Sheets[m_CurrentSheet];

			//! 実行データ初期化
			if ( m_PC < 0 ){
				m_Commands.Reset();
				sheet->GetCommandList( m_Commands );
				m_PC	= 0;
			}

			sm->AdvancePhase();
		}
		break;
	//! メイン
	case Main:
		{
			const FNaEventCommand*	cmd;
			bool	isBreak = false;

			while ( !isBreak ){
				if ( m_Commands.IsValidIndex( m_PC ) ){
					cmd		= m_Commands[m_PC];
					isBreak	= ParseCommand( sm, cmd );
				}
				else {
					sm->SetPhase( End );
					break;
				}
			}
		}
		break;
	//! 終了
	case End:
		sm->ChangeState( EState::Idle );
		break;
	}
}

//! 
void UNaEventManager::ProcMessage( UNaStateMachine* sm )
{
	enum EPhase
	{
		//! 初期化
		Init,
		//! メイン処理
		Main,
		//! 終了
		End
	};
	UNaGameInstance*	gi = UNaGameInstance::Get( m_World->GetWorldContext() );

	switch ( sm->GetPhase() ){
	//! 初期化
	case Init:
		if ( !m_UIASkit ){
			UClass*	cls = gi->FindUI( "Skit" );

			if ( cls ){
				m_UIASkit	= m_World->GetWorldContext()->SpawnActor<ANaSkitAgent>( cls );
			}
		}

		//! メッセージ表示
		if ( m_UIASkit ){
			const FNaEventCommand*	cmd = m_Commands[m_PC];
			
			m_UIASkit->ShowMessage( FText::FromString( cmd->Arg0 ) );
			sm->AdvancePhase();
		}
		else {
			sm->SetPhase( End );
		}
		break;
	//! メイン
	case Main:
		if ( m_UIASkit->WaitForAction() ){
			sm->AdvancePhase();
		}
		break;
	//! 終了
	case End:
		m_PC++;
		sm->ChangeState( EState::Playing );
		break;
	}
}

//! 
void UNaEventManager::ProcSelection( UNaStateMachine* sm )
{
}

//! コマンド解析
bool UNaEventManager::ParseCommand( UNaStateMachine* sm, const FNaEventCommand* cmd )
{
	switch ( cmd->Code ){
	//! UI系
	case ENaEventCode::Message:
		sm->ChangeState( EState::Message );
		GEngine->AddOnScreenDebugMessage( -1, 1.0, FColor::Cyan, cmd->Arg0 );
		break;
	case ENaEventCode::Selection:
		GEngine->AddOnScreenDebugMessage( -1, 1.0, FColor::Green, cmd->Arg0 );
		m_PC++;
		break;
	case ENaEventCode::Shop:
		GEngine->AddOnScreenDebugMessage( -1, 1.0, FColor::Yellow, cmd->Arg0 );
		m_PC++;
		break;
	//! 演算系
	//! 制御系
	}

	return true;
}
