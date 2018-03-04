// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "NaEntityManipulator.generated.h"

class UNaEntityCharacter;

/**
 * �G���e�B�e�B������
 */
UCLASS(abstract)
class NANRPG_API UNaEntityManipulator : public UObject
{
	GENERATED_BODY()
	
public:
	// ����^�[�Q�b�g�ݒ�
	void	BindTarget( UNaEntityCharacter* target );

	// �X�V
	void	UpdateControl( float DeltaTime );

	// �s���I��
	bool	IsEndAction() const	{return true;}

protected:
	// �X�V����
	virtual void	OnControl( float DeltaTime );

	//! 
	UWorld*	GetWorldContext() const;

public:
	
protected:
	UPROPERTY()
	UNaEntityCharacter*	m_Target;
};