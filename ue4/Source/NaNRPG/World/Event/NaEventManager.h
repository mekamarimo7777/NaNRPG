// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Assets/Entity/NaEntityDataTable.h"
#include "Assets/Event/NaEventAsset.h"

#include "World/NaWorld.h"

#include "UI/Actor/Event/NaSkitAgent.h"

#include "NaEventManager.generated.h"

//! 前方宣言
class UNaStateMachine;

//! 
enum class ENaEventParam
{
	//! なし
	None,
	//! 数値
	Number,
	//! 文字列
	String,
	//! テキストID
	TextSymbol,
	//! グローバル変数
	GlobalVariable,
	//! グローバルフラグ
	GlobalFlag,
	//! エンティティ変数
	EntityVariable,
	//! ローカル変数
	LocalVariable,
	//! システム定数
	SystemConst,
};

//! イベントパラメータ
struct FNaEventParam
{
	//! 種別
	ENaEventParam	Type;
	//! 名前（変数系）
	FName			Name;
	//! 値
	FString			Value;
};

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
	void	ProcEvent( UNaStateMachine* sm, float DeltaTime );
	//! 
	void	ProcMessage( UNaStateMachine* sm, float DeltaTime );
	//! 
	void	ProcSelection( UNaStateMachine* sm, float DeltaTime );

	//! コマンド解析
	bool	ParseCommand( UNaStateMachine* sm, const FNaEventCommand* cmd );

	//! SkitAgent生成
	void	SpawnSkitAgent();
	//! メッセージUI消去
	void	CloseMessage();

	//! 
	void	ParseEventParam( FString arg, FNaEventParam& outVal );
	//! 変数書き換え
	void	StoreVariable( FNaEventParam& dst, FNaEventParam& src );

public:
	
protected:
	//! 
	UPROPERTY()
	UNaWorld*	m_World;

	// ステート管理
	UPROPERTY(Transient)
	UNaStateMachine*		m_SM;

	//! イベント
	UPROPERTY(Transient)
	const UNaEventAsset*	m_Event;
	//! シート
	int32					m_CurrentSheet;

	//! 実行中データ
	TArray<FNaEventCommand*>	m_Commands;
	//! 実行中コード
	int32						m_PC;

	//! 寸劇用
	UPROPERTY(Transient)
	ANaSkitAgent*	m_UIASkit;
};
