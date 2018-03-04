// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaEntityManipulator.h"

#include "NaPlayerManipulator.generated.h"

class UNaStateMachine;
class ANaMenuAgent;

/**
 * �v���C���[����Ǘ�
 */
UCLASS()
class NANRPG_API UNaPlayerManipulator : public UNaEntityManipulator
{
	GENERATED_BODY()
	
public:

protected:
	// �X�V����
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
	//! �C�x���g���s��
	void	ProcEvent( UNaStateMachine* sm, float DeltaTime );

	//! ���j���[UI����
	void	CreateMenuAgent();
	//! ���j���[UI�폜
	void	DisposeMenuAgent();

public:
	
protected:
	//! ���j���[���UI�A�N�^
	UPROPERTY()
	ANaMenuAgent*	m_UIAMenu;
};
