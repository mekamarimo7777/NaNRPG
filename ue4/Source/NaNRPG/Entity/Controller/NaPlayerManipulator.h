// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaEntityManipulator.h"

#include "NaPlayerManipulator.generated.h"

class UNaStateMachine;
class ANaMenuAgent;

/**
 * プレイヤー操作管理
 */
UCLASS()
class NANRPG_API UNaPlayerManipulator : public UNaEntityManipulator
{
	GENERATED_BODY()
	
public:

protected:
	// 更新処理
	virtual void	OnControl( float DeltaTime );

	//
	void	ProcWaiting( UNaStateMachine* sm, float DeltaTime );
	//
	void	ProcAction( UNaStateMachine* sm, float DeltaTime );
	//
	void	ProcMenu( UNaStateMachine* sm, float DeltaTime );
	//
	void	ProcInventory( UNaStateMachine* sm, float DeltaTime );
	//
	void	ProcEquipment( UNaStateMachine* sm, float DeltaTime );
	//! イベント実行中
	void	ProcEvent( UNaStateMachine* sm, float DeltaTime );

	//! メニューUI生成
	void	CreateMenuAgent();
	//! メニューUI削除
	void	DisposeMenuAgent();

public:
	
protected:
	//! メニュー画面UIアクタ
	UPROPERTY()
	ANaMenuAgent*	m_UIAMenu;
};
