// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "Utility/Components/NaStateMachine.h"

#include "NaUIAgent.generated.h"

UCLASS(abstract)
class NANRPG_API ANaUIAgent : public AActor
{
	GENERATED_BODY()
	
public:
	//! �R���X�g���N�^
	ANaUIAgent( const FObjectInitializer& ObjectInitializer );

	//! 
	virtual void	BeginPlay() override;
	//! �X�V
	virtual void	Tick( float DeltaSeconds ) override;

	//! �J�n
	virtual void	Start( int32 param = 0 ){}
	//! �I��
	virtual void	Exit(){}

	//! ���s�҂�
	virtual bool	WaitForAction();

protected:
	//! �X�V
	virtual void	OnTick( UNaStateMachine* sm, float DeltaTime ){}

protected:
	//! 
	UPROPERTY()
	UNaStateMachine*	m_StateMachine;
	//! 
	bool				m_IsBusy;
};
