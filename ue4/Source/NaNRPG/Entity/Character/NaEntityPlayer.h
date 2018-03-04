// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Entity/Character/NaEntityCharacter.h"

#include "NaEntityPlayer.generated.h"

class ANaCharacter;
class UUserWidget;

//! プレイヤー専用情報
struct FNaPlayerProperty
{

};

/**
 * 
 */
UCLASS()
class NANRPG_API UNaEntityPlayer : public UNaEntityCharacter
{
	GENERATED_BODY()

public:

protected:
	//
	virtual void	OnAwake() override;
	//
	virtual void	OnSpawn() override;

public:
	
protected:
	//! プレイヤー専用情報
	FNaPlayerProperty	m_PlayerProp;

	UPROPERTY(Transient)
	class UNaPopupDamage*	m_pDialog;
};
