// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "GameModeAtlas.h"

#include "Actor/NaPawn.h"

#include "Controller/MyPlayerController.h"

// コンストラクタ //
AGameModeAtlas::AGameModeAtlas(const FObjectInitializer& ObjectInitializer)
: Super( ObjectInitializer )
, m_State( -1 )
{
	DefaultPawnClass		= ANaPawn::StaticClass();
	PlayerControllerClass	= AMyPlayerController::StaticClass();
}

// 
void AGameModeAtlas::BeginPlay()
{
	Super::BeginPlay();

	ChangeState( 0 );
}

//
void AGameModeAtlas::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );

	switch ( m_State ){
	case 0:
		ProcMain( DeltaTime );
		break;
	}
}

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//
void AGameModeAtlas::ChangeState(int32 state, int32 param, bool immediate)
{
	m_State			= state;
	m_StateParam	= param;
	m_StateStep		= 0;
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//
void AGameModeAtlas::ProcMain(float DeltaTime)
{
	enum StateStep
	{
		Start,
		Main,
		End
	};

	switch ( m_StateStep ){
	case Start:
		m_StateStep++;
		break;

	case Main:
		break;

	case End:
		break;
	}
}
