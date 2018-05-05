// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaEntityPlayer.h"

#include "Actor/Entity/Character/NaCharacter.h"

#include "UI/WIdgets/Common/NaPopupDamage.h"

#include "Entity/INaEntityFactory.h"
#include "Entity/Controller/NaPlayerManipulator.h"

#include "HUD/NaGameHUD.h"

#include "GameMode/NaMainGameMode.h"


//
void UNaEntityPlayer::OnAwake()
{
	Super::OnAwake();
}

//
void UNaEntityPlayer::OnSpawn()
{
	Super::OnSpawn();

	m_Manipulator	= NewObject<UNaPlayerManipulator>();
	m_Manipulator->BindTarget( this );

	m_Status.HP	= m_Status.HPMax	= 50;

	m_Status.BaseParam.Str	= 10;
}
