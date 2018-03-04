// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaWorld.h"

#include "NaGameWorld.generated.h"

/**
 * �����^�C�����[���h
 */
UCLASS()
class NANRPG_API UNaGameWorld : public UNaWorld
{
	GENERATED_BODY()

public:
	
public:
	// �R���X�g���N�^
	UNaGameWorld();

	//! ������
	virtual void	Initialize( UWorld* world ) override;
	//! 
	virtual void	Tick( float DeltaTime ) override;

	//! �C�x���g�}�l�[�W���擾
	virtual UNaEventManager*	GetEventManager() override	{ return m_EventManager; }

protected:

public:
	
protected:
	//! �C�x���g�}�l�[�W��
	UPROPERTY()
	UNaEventManager*	m_EventManager;

};
