// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Assets/Entity/NaEntityDataTable.h"
#include "Assets/Event/NaEventAsset.h"

#include "World/NaWorld.h"

#include "UI/Actor/Event/NaSkitAgent.h"

#include "NaEventManager.generated.h"

//! 前方宣言
class UNaStateMachine;

/**
 * イベントマネージャ
 */
UCLASS()
class NANRPG_API UNaEventManager : public UObject
{
	GENERATED_BODY()

	enum EState
	{
		//! 実行なし
		Idle,
		//! 実行中
		Playing,
		//! メッセージ表示
		Message,
		//! 選択肢表示
		Selection,
	};

public:
	
public:
	//! コンストラクタ
	UNaEventManager();

	//！初期化
	void	Initialize( UNaWorld* world );
	//! 更新
	void	Tick( float DeltaTime );

	//! 実行要求
	void	PlayEvent( const UNaEventAsset* evt, int32 sheet );
	//! 実行中判定
	bool	IsPlaying() const;

protected:
	//! 
	void	ProcEvent( UNaStateMachine* sm );
	//! 
	void	ProcMessage( UNaStateMachine* sm );
	//! 
	void	ProcSelection( UNaStateMachine* sm );

	//! コマンド解析
	bool	ParseCommand( UNaStateMachine* sm, const FNaEventCommand* cmd );

public:
	
protected:
	//! 
	UPROPERTY()
	UNaWorld*	m_World;

	// ステート管理
	UPROPERTY(Transient)
	UNaStateMachine*		m_StateMachine;

	//! イベント
	UPROPERTY(Transient)
	const UNaEventAsset*	m_Event;
	//! シート
	int32					m_CurrentSheet;

	//! 実行中データ
	TArray<FNaEventCommand*>	m_Commands;
	//! 実行中コード
	int32						m_PC;

	//! 
	UPROPERTY(Transient)
	ANaSkitAgent*	m_UIASkit;
};
