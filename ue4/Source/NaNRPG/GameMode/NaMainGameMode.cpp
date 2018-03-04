// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaMainGameMode.h"

#include "Actor/NaPawn.h"

#include "World/NaGameWorld.h"

#include "Assets/NaAssetLibrary.h"
#include "Database/NaGameDatabase.h"

#include "HUD/NaGameHUD.h"

#include "UI/Widgets/HUD_old/NaMainHUD.h"

#include "Entity/INaEntityFactory.h"

#include "Controller/MyPlayerController.h"

// コンストラクタ //
ANaMainGameMode::ANaMainGameMode(const FObjectInitializer& ObjectInitializer)
: Super( ObjectInitializer )
, m_State( -1 )
{
	DefaultPawnClass		= ANaPawn::StaticClass();
	PlayerControllerClass	= AMyPlayerController::StaticClass();
}

// 
void ANaMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld*	const		world = GetWorld();
	APlayerController*	pc = world->GetFirstPlayerController();

#if WITH_EDITOR
	{// 直接エディタから来た場合のGameDatabase読み込み
		UNaGameDatabase*	db = UNaGameDatabase::GetDB();

		db->LoadDB( "test" );

		if ( !db->GetPlayer() ){
			UNaAssetLibrary*			alib = UNaAssetLibrary::Get();
			UNaEntityPlayer*			player;
			const FNaEntityDataAsset*	asset;

			player	= INaEntityFactory::NewEntity<UNaEntityPlayer>( ENaEntity::Player );

			asset	= alib->FindEntityAsset( "Player" );
			player->CreateFromAsset( *asset );
			db->RegisterPlayer( player );
		}
	}
#endif

	{// ワールド生成
		m_NaWorld	= LoadWorld( 0, "TestWorld" );

		if ( world ){
			m_pCamera	= world->SpawnActor<ANaCameraActor>( CameraActorClass );
			pc->SetViewTargetWithBlend( m_pCamera );

			m_pMapActor	= world->SpawnActor<ANaWorldActor>( WorldActorClass );
	//		m_pMapActor->LoadMap( 0 );
			m_pMapActor->SetCamera( m_pCamera );
			m_pMapActor->AssignWorld( m_NaWorld );
		}
	}

	ChangeState( 0 );
}

//
void ANaMainGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	switch ( EndPlayReason ){
	case EEndPlayReason::EndPlayInEditor:
		if ( m_pMapActor ){
			m_pMapActor->CloseWorld();
		}
		break;
	}

	Super::EndPlay( EndPlayReason );
}

//
void ANaMainGameMode::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );

	switch ( m_State ){
	case 0:
		ProcMain( DeltaTime );
		break;
	}

	m_pCamera->Update( DeltaTime );
}

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//
void ANaMainGameMode::ChangeState(int32 state, int32 param, bool immediate)
{
	m_State			= state;
	m_StateParam	= param;
	m_StateStep		= 0;
}

//
void ANaMainGameMode::TravelToWorld(int32 worldID)
{
	m_pMapActor->LoadMap( worldID );
}

//! ワールド読み込み（今後どこかのstaticに移動）
UNaWorld* ANaMainGameMode::LoadWorld( int32 uid, FName worldID )
{
	UWorld*	const		world = GetWorld();
	APlayerController*	pc = world->GetFirstPlayerController();
	FIntVector			ppos( 0, 0, 64 );
	UNaWorld*			naw = nullptr;

//@	naw	= UNaWorld::Open( uid );

	if ( !naw ){
		UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
		UNaGameDatabase*	db = UNaGameDatabase::GetDB();
		UNaWorldAsset*		worldAsset;
		UNaEntity*			entity;

		naw	= NewObject<UNaGameWorld>();
		naw->Initialize( GetWorld() );

		worldAsset	= alib->FindWorldAsset( worldID );
		naw->CreateWorld( uid, worldAsset );

		// プレイヤー配置
		entity	= db->GetPlayer();
		naw->SetCurrentPosition( ppos );
		naw->SpawnEntity( entity, ppos );
	}
/*
	if ( m_pWorld ){
		UNaGameDatabase*	db = UNaGameDatabase::GetDB();
		UNaEntity*			entity;

		m_pWorld->DataAsset	= m_pDataAsset;
		if ( mapID == 2 ){
			m_pWorld->SetChunkLimit( FIntVector(0, 0, -1), FIntVector(4, 4, 1) );
		}
		if ( mapID == 3 ){
			UNaMapAsset*	asset = NewObject<UNaMapAsset>();
			UNaMap*			map = NewObject<UNaMap>();

			asset->CreateSimpleRandomMap( FIntVector( 4, 4, 1 ) );
			map->Instantiate( asset );

			m_pWorld->EntryMap( FIntVector( -2, -2, 0 ), map );
		}
		m_pWorld->SetCurrentPosition( ppos );
	
		// プレイヤー配置
		entity	= db->m_pPlayer;
		entity->SetWorldPosition( ppos );
		m_pWorld->SpawnEntity( entity );

		// 
//		entity	= INaEntityFactory::NewEntity( ENaEntity::GLOBAL_SPAWNER );
//		entity->SetStage( ENaEntityStage::Transient );
//		m_pWorld->SpawnEntity( entity );
	}

	*/
	return naw;
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//
void ANaMainGameMode::ProcMain(float DeltaTime)
{
	enum StateStep
	{
		Start,
		Main,
		End
	};

	switch ( m_StateStep ){
	case Start:
		{
			UNaEntityCharacter*	player = m_NaWorld->GetPlayer();
			ANaGameHUD*			hud;
//			UNaMainHUD*			mainHUD;

			hud		= player->GetHUD();
//			mainHUD	= hud->GetMainHUD();
//			mainHUD->BindTarget( player );

			m_StateStep++;
		}
		break;

	case Main:
		if ( m_NaWorld ){
			m_NaWorld->Tick( DeltaTime );
		}
		break;

	case End:
		break;
	}
}
