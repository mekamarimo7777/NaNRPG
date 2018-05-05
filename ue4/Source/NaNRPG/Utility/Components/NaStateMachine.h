// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaStateMachine.generated.h"

class UNaStateMachine;

//! ステート処理デリゲート
DECLARE_DELEGATE_TwoParams( FNaStateDelegate, UNaStateMachine*, float );

/**
 * 汎用ステートマシン
 */
UCLASS()
class NANRPG_API UNaStateMachine : public UObject
{
	GENERATED_BODY()
	
public:
	//! コンストラクタ
	UNaStateMachine( const FObjectInitializer& ObjectInitializer );

	//! ステート登録
	void	RegisterState( int32 state, FNaStateDelegate func );
	//! ステート登録（メンバ指定）
	template <typename ClassType, typename... VarTypes>
	inline void RegisterState( int32 state, ClassType* InUserObject, typename TMemFunPtrType<false, ClassType, void (UNaStateMachine*, float, VarTypes...)>::Type InFunc, VarTypes... Vars )
	{
		FNaStateDelegate	func;

		func.BindUObject( InUserObject, InFunc, Vars... );
		RegisterState( state, func );
	}
	//! ステート解除
	void	UnregisterState( int32 state );

	//! 実行
	void	Execute( float DeltaTime );

	//! ステート変更
	void	ChangeState(int32 state, int32 param = 0, bool immediate = false);
	//! ステート取得
	int32	GetState() const		{ return m_State; }
	//! ステートパラメータ取得
	int32	GetParam()				{ return m_StateParam; }
	//! 再実行予約
	void	Again()					{ m_Again = true; }

	//! フェーズ設定
	void	SetPhase(int32 phase)	{ m_StatePhase = phase; }
	//! フェーズ取得
	int32	GetPhase() const		{ return m_StatePhase; }
	//! フェーズ進行
	void	Advance()			{ m_StatePhase++; }

protected:

public:
	
protected:
	//! ステート
	TArray<FNaStateDelegate>	m_StateFunc;

	//! 現在ステート
	int32	m_State;
	//! ステートパラメータ
	int32	m_StateParam;
	//! フェイズ
	int32	m_StatePhase;

	//! 次回ステート
	int32	m_NextState;
	//! 次回ステートパラメータ
	int32	m_NextStateParam;

	//! 再実行
	bool	m_Again;
};
