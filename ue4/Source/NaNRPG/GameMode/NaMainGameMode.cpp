// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaMainGameMode.h"

#include "Actor/NaPawn.h"

#include "World/NaGameWorld.h"

#include "Assets/NaAssetLibrary.h"
#include "Database/NaGameDatabase.h"

#include "HUD/NaGameHUD.h"

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
