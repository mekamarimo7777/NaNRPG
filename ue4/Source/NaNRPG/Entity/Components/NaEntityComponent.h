// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaEntityComponent.generated.h"

class UNaEntity;

/**
 * エンティティコンポーネント基底
 */
UCLASS()
class NANRPG_API UNaEntityComponent : public UObject
{
	GENERATED_BODY()
	
public:
	//! オーナー設定
	void		SetOwner( UNaEntity* owner )	{ m_Owner = owner; }
	//! オーナー取得
	UNaEntity*	GetOwner() const				{ return m_Owner; }

protected:

public:
	
protected:
	UPROPERTY(Transient)
	UNaEntity*	m_Owner;
};
