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
	// �R���X�g���N�^
	UNaTurnActionComponent( const FObjectInitializer& ObjectInitializer );

	// �^�[���J�n����
	virtual void	BeginTurn();
	// �^�[�����s
	virtual void	ExecuteTurn( float DeltaTime );
	// �^�[���I������
	virtual void	EndTurn();

	// �^�[���I������
	bool	IsEndTurn() const	{ return m_IsEndTurn; }

	// 
	void	SetSpeed( int32 speed )		{ m_Speed = speed; }

	// �E�F�C�g�^�C���擾
	int32	GetWaitTime() const	{return m_WaitTime;}
	// �E�F�C�g�^�C��������
	void	ResetWaitTime()		{m_WaitTime	= 10000 / m_Speed;}
	// �E�F�C�g�^�C������
	void	DecreaseWaitTime(int32 value)	{m_WaitTime	-= value;}

	//! �^�[���J�n�C�x���g
	FNaTurnActionBeginTurnEvent&	BeginTurnEvent()	{ return m_BeginTurnEvent; }
	//! �^�[���X�V�C�x���g
	FNaTurnActionUpdateTurnEvent&	UpdateTurnEvent()	{ return m_UpdateTurnEvent; }

protected:

public:
	
public:
	//
	int32	m_Speed;			// �s�����x //
	int32	m_WaitTime;			// �҂����� //
	bool	m_IsEndTurn;		// 
	
	//! �^�[���J�n�C�x���g
	FNaTurnActionBeginTurnEvent		m_BeginTurnEvent;
	//! �^�[���X�V�C�x���g
	FNaTurnActionUpdateTurnEvent	m_UpdateTurnEvent;
};
