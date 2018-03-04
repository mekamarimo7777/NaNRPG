// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaEntityComponent.h"

#include "NaStateMachine.generated.h"

/**
 * 汎用ステートマシン
 */
UCLASS()
class NANRPG_API UNaStateMachine : public UNaEntityComponent
{
	GENERATED_BODY()
	
public:
	//!
	UNaStateMachine( const FObjectInitializer& ObjectInitializer );

	//! ステート変更
	void	ChangeState(int32 state, int32 param = 0, bool immediate = false);
	//! ステート取得
	int32	GetState() const		{ return m_State; }
	//! ステートパラメータ取得
	int32	GetParam()				{ return m_StateParam; }
	//! 
//	void	PushState(int32 state);
	//! 
//	void	PopState(int32 state);

	//! 
	void	SetPhase(int32 phase)	{ m_StatePhase = phase; }
	//! 
	int32	GetPhase() const		{ return m_StatePhase; }
	//! 
	void	AdvancePhase()			{ m_StatePhase++; }

	//! 更新
	void	Update( float DeltaTime );

protected:

public:
	
protected:
	int32	m_State;
	int32	m_StateParam;
	int32	m_StatePhase;

	int32	m_NextState;
	int32	m_NextStateParam;

	bool	m_Continue;
};
