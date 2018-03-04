// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaEntityComponent.generated.h"

class UNaEntity;

/**
 * �G���e�B�e�B�R���|�[�l���g���
 */
UCLASS()
class NANRPG_API UNaEntityComponent : public UObject
{
	GENERATED_BODY()
	
public:
	//! �I�[�i�[�ݒ�
	void		SetOwner( UNaEntity* owner )	{ m_Owner = owner; }
	//! �I�[�i�[�擾
	UNaEntity*	GetOwner() const				{ return m_Owner; }

protected:

public:
	
protected:
	UPROPERTY(Transient)
	UNaEntity*	m_Owner;
};
