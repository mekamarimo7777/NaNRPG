// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Entity/NaEntity.h"

#include "Database/NaInventory.h"

#include "NaEntityCharacter.generated.h"

class ANaActorBase;

//! キャラクター情報（DataTableからコンバート）
USTRUCT()
struct FNaCharacterProperty
{
	GENERATED_BODY()
};

/**
 * 
 */
UCLASS()
class NANRPG_API UNaEntityCharacter : public UNaEntity
{
	GENERATED_BODY()
	
public:
	//!
	enum EState : int32
	{
		//! 基本
		Waiting,		// 待ち
		Action,			// 行動受付中
		SelectTarget,	// ターゲット選択
		
		//! メニュー系
		Menu,
		Inventory,
		Equipment,
		Skill,
		Status,

		//! イベント
		Event,

		EndTurn
	};

public:
	//==================================================
	// ターンアクション
	//==================================================
	//!
	virtual UNaTurnActionComponent*	GetTurnAction() const	{ return m_TurnAction; }

	//
	virtual void	SetWorldPosition(const FIntVector& pos) override;
	//
	virtual void	MoveToWorldPosition(const FIntVector& pos) override;

	//
	void			Attack();
	//
	virtual void	TakeDamage( int32 value, UNaEntity* actor ) override;
	//! アイテム使用
	void			UseItem( UNaItem* item, ENaActionTrigger::Type action );

	//
	ANaActorBase*		GetActor()				{return m_Actor;}
	//
	UNaInventory*		GetInventory() const	{ return m_Inventory; }
	//!
	FNaCharacterStatus*	GetStatus()				{ return &m_Status; }

	//! 所持金取得
	int32	GetMoney() const		{ return m_Inventory->GetMoney(); }

	//
	virtual UNaEntityManipulator*	GetManipulator() const	{ return m_Manipulator; }

	//! 干渉対象収集
	void	GatherInteractTarget( TArray<UNaEntity*>& outVal );
	//! 攻撃対象収集
	void	EnumAttackTarget( TArray<UNaEntity*>& outVal );

	//! エンティティ情報生成
	virtual void		CreateFromAsset( const FNaEntityDataAsset& asset ) override;

	//! アイテム情報セット
	virtual void		SetItemProperty( UNaItem* item ) override;
	//! アイテム情報取得
	virtual UNaItem*	GetItemProperty() override	{ return m_ItemProperty; }

	//! アイテム装備
	void	EquipItem( int32 index, UNaItem* item );
	//! 装備リスト取得
	const TArray<FNaCharacterEquipPart>&	GetEquipments() const	{ return m_Equipments; }

protected:
	// 生成後の初期化
	virtual void	OnAwake() override;
	//
	virtual void	OnSpawn() override;
	//
	virtual void	OnEnter() override;
	//
	virtual void	OnLeave() override;

	// ターン開始
	UFUNCTION()
	void	OnBeginTurn();
	// ターン更新
	UFUNCTION()
	void	OnUpdateTurn( float DeltaTime );

	//! シリアライズ処理
	virtual void	OnSerialize( FArchive& ar ) override;

public:

protected:
	// マップアクター
	UPROPERTY(Transient)
	ANaActorBase*	m_Actor;

	// components
	// 操作モジュール
	UPROPERTY()
	UNaEntityManipulator*		m_Manipulator;
	// ターンアクション
	UPROPERTY()
	UNaTurnActionComponent*		m_TurnAction;

	//! キャラクター情報
	UPROPERTY()
	FNaCharacterProperty		m_Property;
	//! キャラクターステータス
	UPROPERTY()
	FNaCharacterStatus			m_Status;
	//! 所持スキル
	UPROPERTY()
	TArray<FNaCharacterSkill>	m_Skills;
	//! 所持才能
	UPROPERTY()
	TArray<FNaCharacterTalent>	m_Talents;

	//! アイテム化情報
	UPROPERTY()
	UNaItem*		m_ItemProperty;
	//! インベントリ
	UPROPERTY()
	UNaInventory*	m_Inventory;
	//! 装備品
	UPROPERTY()
	TArray<FNaCharacterEquipPart>	m_Equipments;

	//! 総行動ターン数
	UPROPERTY()
	uint32			m_TotalTurn;
};
