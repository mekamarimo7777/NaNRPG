// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Entity/NaEntity.h"

#include "Database/Item/NaItem.h"

#include "NaEntitySpawnManager.generated.h"

/**
 * ワンダリングモンスタースポナー
 */
UCLASS()
class NANRPG_API UNaEntitySpawnManager : public UNaEntity
{
	GENERATED_BODY()
	
public:
	//==================================================
	// ターンアクション
	//==================================================
	//!
	virtual UNaTurnActionComponent*	GetTurnAction() const	{ return m_TurnAction; }

	// 無形エンティティ（チャンクに所属しない）
	virtual bool	IsIntangible() const override	{return true;}

protected:
	// 生成後の初期化
	virtual void	OnAwake() override;

	// ターン開始
	UFUNCTION()
	void	OnBeginTurn();
	// ターン更新
	UFUNCTION()
	void	OnUpdateTurn( float DeltaTime );

	//! アイテム生成（仮）
	UNaItem*	GenerateItem( int32 intensity, int32 typeMask );


public:
	
protected:
	// components
	// ターンアクション
	UPROPERTY()
	UNaTurnActionComponent*	m_TurnAction;

	int32	m_SpawnMax;
};
