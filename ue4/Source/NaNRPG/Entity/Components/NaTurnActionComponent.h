// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaEntityComponent.h"

#include "NaTurnActionComponent.generated.h"

//
DECLARE_DELEGATE( FNaTurnActionBeginTurnEvent );
//
DECLARE_DELEGATE_OneParam( FNaTurnActionUpdateTurnEvent, float );

/**
 * 
 */
UCLASS()
class NANRPG_API UNaTurnActionComponent : public UNaEntityComponent
{
	GENERATED_BODY()
	
public:
	// コンストラクタ
	UNaTurnActionComponent( const FObjectInitializer& ObjectInitializer );

	// ターン開始処理
	virtual void	BeginTurn();
	// ターン実行
	virtual void	ExecuteTurn( float DeltaTime );
	// ターン終了処理
	virtual void	EndTurn();

	// ターン終了判定
	bool	IsEndTurn() const	{ return m_IsEndTurn; }

	// 
	void	SetSpeed( int32 speed )		{ m_Speed = speed; }

	// ウェイトタイム取得
	int32	GetWaitTime() const	{return m_WaitTime;}
	// ウェイトタイム初期化
	void	ResetWaitTime()		{m_WaitTime	= 10000 / m_Speed;}
	// ウェイトタイム減少
	void	DecreaseWaitTime(int32 value)	{m_WaitTime	-= value;}

	//! ターン開始イベント
	FNaTurnActionBeginTurnEvent&	BeginTurnEvent()	{ return m_BeginTurnEvent; }
	//! ターン更新イベント
	FNaTurnActionUpdateTurnEvent&	UpdateTurnEvent()	{ return m_UpdateTurnEvent; }

protected:

public:
	
public:
	//
	int32	m_Speed;			// 行動速度 //
	int32	m_WaitTime;			// 待ち時間 //
	bool	m_IsEndTurn;		// 
	
	//! ターン開始イベント
	FNaTurnActionBeginTurnEvent		m_BeginTurnEvent;
	//! ターン更新イベント
	FNaTurnActionUpdateTurnEvent	m_UpdateTurnEvent;
};
