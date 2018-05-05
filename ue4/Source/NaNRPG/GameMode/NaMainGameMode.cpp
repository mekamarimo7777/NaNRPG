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

//! コンストラクタ
ANaMainGameMode::ANaMainGameMode(const FObjectInitializer& ObjectInitializer)
: Super( ObjectInitializer )
{
	DefaultPawnClass		= ANaPawn::StaticClass();
	PlayerControllerClass	= AMyPlayerController::StaticClass();
}

//! 開始処理
void ANaMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld*	const	world = GetWorld();

#if WITH_EDITOR
	{// 直接エディタから来た場合のGameDatabase読み込み
		UNaGameDatabase*	db = UNaGameDatabase::GetDB();

		db->LoadDB( "test" );

		//! 初期データ構築（そのうちアセットからの生成に変更）
		if ( !db->GetPlayer() ){
			UNaAssetLibrary*			alib = UNaAssetLibrary::Get();
			UNaEntityPlayer*			player;
			const FNaEntityDataAsset*	asset;

			//! プレイヤーデータ生成
			player	= INaEntityFactory::NewEntity<UNaEntityPlayer>( ENaEntity::Player );

			asset	= alib->FindEntityAsset( "Player" );
			player->CreateFromAsset( *asset );
			player->SetWorldID( "Home" );
			player->SetWorldPosition( FIntVector( 0, 0, 64 ) );
			db->RegisterPlayer( player );
		}
	}
#endif

	//! カメラ生成
	if ( world ){
		APlayerController*	pc = world->GetFirstPlayerController();

		m_Camera	= world->SpawnActor<ANaCameraActor>( CameraActorClass );
		check( m_Camera );

		pc->SetViewTargetWithBlend( m_Camera );
	}

	//! ステート管理
	m_SM	= NewObject<UNaStateMachine>();
	if ( m_SM ){
		m_SM->RegisterState( EState::Main, this, &ANaMainGameMode::ProcMain );
	}
	m_SM->ChangeState( EState::Main );

/*	{// ワールド生成
		m_NaWorld	= LoadWorld( 0, "TestWorld" );

		if ( world ){
			m_Camera	= world->SpawnActor<ANaCameraActor>( CameraActorClass );
			pc->SetViewTargetWithBlend( m_Camera );

			m_pMapActor	= world->SpawnActor<ANaWorldActor>( WorldActorClass );
	//		m_pMapActor->LoadMap( 0 );
			m_pMapActor->SetCamera( m_Camera );
			m_pMapActor->AssignWorld( m_NaWorld );
		}
	}*/
}

//! 終了処理
void ANaMainGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	switch ( EndPlayReason ){
	case EEndPlayReason::EndPlayInEditor:
//		if ( m_pMapActor ){
//			m_pMapActor->CloseWorld();
//		}
		break;
	}

	Super::EndPlay( EndPlayReason );
}

//! 更新
void ANaMainGameMode::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );

	m_SM->Execute( DeltaTime );

	m_Camera->Update( DeltaTime );
}

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//
void ANaMainGameMode::TravelToWorld(int32 worldID)
{
//	m_pMapActor->LoadMap( worldID );
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
/*		UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
		UNaGameDatabase*	db = UNaGameDatabase::GetDB();
		UNaWorldAsset*		worldAsset;
		UNaEntity*			entity;

		naw	= NewObject<UNaGameWorld>();
		naw->Setup( GetWorld() );

		worldAsset	= alib->FindWorldAsset( worldID );
		naw->CreateWorld( uid, worldAsset );

		// プレイヤー配置
		entity	= db->GetPlayer();
		naw->SetCurrentPosition( ppos );
		naw->SpawnEntity( entity, ppos );*/
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
//! メイン
void ANaMainGameMode::ProcMain( UNaStateMachine* sm, float DeltaTime )
{
	enum EPhase
	{
		Start,
		Main,
		End
	};

	switch ( sm->GetPhase() ){
	case Start:
		//! ワールドアクタ生成
		CreateWorldActor();

		//! ワールド読み込み
		{
			UNaGameDatabase*	db = UNaGameDatabase::GetDB();
			UNaEntityPlayer*	player = db->GetPlayer();
			UNaWorld*			naw;

			naw	= m_WorldActor->OpenWorld( player->GetWorldID() );
		}

		sm->Advance();
		break;

	case Main:
		break;

	case End:
		break;
	}
}

//! ワールド管理アクター生成
void ANaMainGameMode::CreateWorldActor()
{
	UWorld*	world = GetWorld();

	m_WorldActor	= world->SpawnActor<ANaWorldActor>( WorldActorClass );
	check( m_WorldActor );

	m_WorldActor->BindCamera( m_Camera );
}
