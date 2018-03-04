﻿// Fill out your copyright notice in the Description page of Project Settings.

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

/*
	UWorld*	const	world = m_pWorld->GetUWorld();

	//
	m_Actor	= world->SpawnActor<ANaCharacter>();
	m_Actor->Initialize( m_pWorld, this );
//	m_Actor->SetStaticMesh( m_pWorld->DataAsset->TestMesh );
//	m_Actor->SetSkeletalMesh( m_pWorld->DataAsset->TestMesh3, m_pWorld->DataAsset->TestAnimBP, m_pWorld->DataAsset->TestAnim );
	{
		FNaActorDefinition* adef = m_pWorld->DataAsset->FindActorDefinition( "UnityChan" );
		if ( adef ){
			m_Actor->SetActorDefinition( *adef );
		}
	}
	m_Actor->SetWorldPosition( GetWorldPosition(), true );

	m_Speed	= 100;
	m_TurnAction->SetSpeed( 100 );

	m_Group		= 0;
	m_bNoSave	= true;*/
}
