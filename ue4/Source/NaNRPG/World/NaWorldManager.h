// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Misc/NaEnums.h"

#include "Generation/NaWorldGenerator.h"

#include "NaWorldDefinition.h"
#include "NaWorld.h"

#include "NaWorldManager.generated.h"

class ANaWorldActor;
class UNaTurnActionComponent;
class UNaEventManager;

/**
 * ゲームワールドマネージャ
 * 複数ワールドの統括管理・及びランタイム固有処理の実行を行う
 */
UCLASS()
class NANRPG_API UNaWorldManager : public UObject
{
	GENERATED_BODY()

public:
	//! コンストラクタ
	UNaWorldManager();

	//! 初期化
	void	Initialize( ANaWorldActor* actor );

	//! 更新処理
	void	Update( float DeltaTime );

	//! アクションチェインに追加
	void	InsertActionChain( UNaTurnActionComponent* tac );
	void	InsertActionChain( UNaEntity* entity );
	//! アクションチェインから除去
	void	RemoveActionChain( UNaTurnActionComponent* tac );
	void	RemoveActionChain( UNaEntity* entity );

	//! ワールドオープン
	UNaWorld*	OpenWorld( FName id, FName assetID = FName() );
	//! ワールドクローズ
	void		CloseWorld( FName id );
	//! ワールド変更リクエスト
	void		ChangeWorld( FName id );

	//! 準備完了判定
	bool				IsReady() const			{ return m_WorldActor != nullptr; }
	//! イベントマネージャ取得
	UNaEventManager*	GetEventManager() const	{ return m_EventManager; }
	//! アクティブワールド取得
	UNaWorld*			GetActiveWorld() const	{ return m_ActiveWorld; }
	//! UEワールド取得
	UWorld*				GetWorldContext() const	{ return m_WorldContext; }

protected:
	//! 表示ターゲット取得
	UNaEntity*	GetFocusEntity() const;
	
protected:
	//! イベントマネージャ
	UPROPERTY()
	UNaEventManager*	m_EventManager;

	//! オープン中のワールド
	UPROPERTY()
	TArray<UNaWorld*>	m_Worlds;
	//! アクティブワールド（表示するワールド）
	UPROPERTY()
	UNaWorld*			m_ActiveWorld;
	//! ワールド変更リクエスト
	UPROPERTY()
	UNaWorld*			m_RequestWorld;

	//! アクションリスト
	UPROPERTY(Transient)
	TArray<UNaTurnActionComponent*>	m_ActionChain;
	//! カレントアクション
	UPROPERTY(Transient)
	UNaTurnActionComponent*			m_CurrentAction;

	//! ワールド表示管理アクター
	UPROPERTY(Transient)
	ANaWorldActor*		m_WorldActor;
	//! UEワールド
	UPROPERTY(Transient)
	UWorld*				m_WorldContext;
};
