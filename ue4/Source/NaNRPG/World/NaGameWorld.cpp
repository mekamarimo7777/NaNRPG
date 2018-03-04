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

//! èâä˙âª
void UNaGameWorld::Initialize( UWorld* world )
{
	Super::Initialize( world );

	m_EventManager	= NewObject<UNaEventManager>();
	m_EventManager->Initialize( this );
}

//! 
void UNaGameWorld::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if ( m_EventManager ){
		m_EventManager->Tick( DeltaTime );
	}
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
