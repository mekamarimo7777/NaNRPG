// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaGameWorld.h"

#include "World/Event/NaEventManager.h"

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//
UNaGameWorld::UNaGameWorld()
{

}

//! ������
void UNaGameWorld::Setup( ANaWorldActor* actor )
{
	Super::Setup( actor );

	m_EventManager	= NewObject<UNaEventManager>();
	m_EventManager->Initialize( this );
}

//! 
void UNaGameWorld::Update( float DeltaTime )
{
	Super::Update( DeltaTime );

	if ( m_EventManager ){
		m_EventManager->Tick( DeltaTime );
	}
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
