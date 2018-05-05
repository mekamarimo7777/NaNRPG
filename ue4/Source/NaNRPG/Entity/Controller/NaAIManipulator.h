// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaEntityManipulator.h"

#include "NaAIManipulator.generated.h"

/**
 * 
 */
UCLASS()
class NANRPG_API UNaAIManipulator : public UNaEntityManipulator
{
	GENERATED_BODY()
	
public:

protected:
	// çXêVèàóù
	virtual void	OnControl( UNaStateMachine* sm, float DeltaTime );

	//
	void	ProcAction( UNaStateMachine* sm, float DeltaTime );

	//
	void	LockTarget();
	//
	void	MoveTo( ENaDirection dir );

public:
	
protected:
	UPROPERTY()
	UNaEntity*	m_LockTarget;
};
