// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Entity/NaEntity.h"

#include "Database/Item/NaItem.h"

#include "NaEntitySpawnManager.generated.h"

/**
 * �����_�����O�����X�^�[�X�|�i�[
 */
UCLASS()
class NANRPG_API UNaEntitySpawnManager : public UNaEntity
{
	GENERATED_BODY()
	
public:
	//==================================================
	// �^�[���A�N�V����
	//==================================================
	//!
	virtual UNaTurnActionComponent*	GetTurnAction() const	{ return m_TurnAction; }

	// ���`�G���e�B�e�B�i�`�����N�ɏ������Ȃ��j
	virtual bool	IsIntangible() const override	{return true;}

protected:
	// ������̏�����
	virtual void	OnAwake() override;

	// �^�[���J�n
	UFUNCTION()
	void	OnBeginTurn();
	// �^�[���X�V
	UFUNCTION()
	void	OnUpdateTurn( float DeltaTime );

	//! �A�C�e�������i���j
	UNaItem*	GenerateItem( int32 intensity, int32 typeMask );


public:
	
protected:
	// components
	// �^�[���A�N�V����
	UPROPERTY()
	UNaTurnActionComponent*	m_TurnAction;

	int32	m_SpawnMax;
};
