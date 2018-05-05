// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaWorldActor.h"

#include "Assets/NaAssetLibrary.h"

#include "Actor/Entity/NaActorBase.h"
#include "Actor/Entity/Character/NaCharacter.h"

#include "World/NaGameWorld.h"
#include "World/Map/NaMap.h"

#include "Entity/INaEntityFactory.h"
#include "Entity/Character/NaEntityPlayer.h"

#include "Database/NaGameDatabase.h"

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//! 
ANaWorldActor::ANaWorldActor( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer )
{
 	//! 
	PrimaryActorTick.bCanEverTick = true;

	//! ルートコンポーネント
	UStaticMeshComponent*	comp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT("RootComponent") );
	RootComponent	= comp;

	//! ライト
	m_pDirLight	= CreateDefaultSubobject<UDirectionalLightComponent>( TEXT("DiretionalLight") );
	m_pDirLight->SetRelativeRotation( FRotator( -31.0f, 36.0f, -51.0f ) );
	m_pDirLight->AttachToComponent( RootComponent, FAttachmentTransformRules::KeepRelativeTransform );

	//! 遮蔽キャプチャ
	m_CaptureCube	= CreateDefaultSubobject<USceneCaptureComponentCube>( TEXT("OccludeCapture") );
	if ( m_CaptureCube ){
		m_CaptureCube->AttachToComponent( RootComponent, FAttachmentTransformRules::KeepRelativeTransform );
		m_CaptureCube->CaptureSource		= ESceneCaptureSource::SCS_SceneDepth;
		m_CaptureCube->bCaptureEveryFrame	= false;
		m_CaptureCube->bCaptureOnMovement	= false;
	}
	//! ポストプロセス
	m_PostProcess	= CreateDefaultSubobject<UPostProcessComponent>( TEXT("PostProcessCmp") );
	if ( m_PostProcess ){
		m_PostProcess->AttachToComponent( RootComponent, FAttachmentTransformRules::KeepRelativeTransform );
	}

	m_CurrentPos.Z		= INT_MAX;
	m_CurrentChunkPos.Z	= SHORT_MAX;
}

//! 
void ANaWorldActor::BeginPlay()
{
	Super::BeginPlay();

	if ( PostProcessMaterial ){
		m_MIDPost	= UMaterialInstanceDynamic::Create( PostProcessMaterial, this );
		if ( m_MIDPost && m_PostProcess ){
			m_PostProcess->AddOrUpdateBlendable( m_MIDPost );
		}
	}

	SetRenderSize( FIntVector( 3, 3, 0 ), FIntVector( 1, 1, 0 ) );

	//! ステート管理
	m_SM	= NewObject<UNaStateMachine>();
	if ( m_SM ){
		m_SM->RegisterState( EState::Main, this, &ANaWorldActor::ProcMain );
	}
	m_SM->ChangeState( EState::Main );
}
	
//! 
void ANaWorldActor::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

	m_SM->Execute( DeltaSeconds );
}

//! ワールドオープン
UNaWorld* ANaWorldActor::OpenWorld( FName id, FName assetID )
{
	UNaGameDatabase*	db = UNaGameDatabase::GetDB();
	UWorld*	const		world = GetWorld();
	UNaWorld*			naw;

	naw	= NewObject<UNaGameWorld>();

	//! エントリ問い合わせ
	if ( FNaWorldRecord* rec = db->FindWorldEntry( id ) ){
		naw->OpenWorld( rec->DataID );
	}
	else {
		//! 新規生成
		naw->CreateWorld( id, assetID );
		db->RegisterWorldEntry( id, naw->GetDataID() );
	}

	naw->Setup( this );

	m_Worlds.Add( naw );

	if ( !m_ActiveWorld ){
		m_NextWorld	= naw;
	}

	return naw;
}

//! ワールドクローズ
void ANaWorldActor::CloseWorld( FName id )
{
	int32	idx;

	idx	= m_Worlds.IndexOfByPredicate( [id]( UNaWorld* p )
	{
		return p->GetUID() == id;
	});

	if ( idx >= 0 ){
		UNaWorld*	naw = m_Worlds[idx];

		naw->CloseWorld();

		if ( naw == m_NextWorld ){
			m_NextWorld		= nullptr;
		}
		if ( naw == m_ActiveWorld ){
			m_ActiveWorld	= nullptr;
		}

		m_Worlds.RemoveAt( idx );
	}
}

//! アクティブワールド変更リクエスト
void ANaWorldActor::SwitchWorld( FName id )
{
	int32	idx;

	idx	= m_Worlds.IndexOfByPredicate( [id]( UNaWorld* p )
	{
		return p->GetUID() == id;
	});

	if ( idx >= 0 ){
		m_NextWorld	= m_Worlds[idx];
	}
}

//! 表示チャンク範囲設定
void ANaWorldActor::SetRenderSize( FIntVector size, FIntVector margin )
{
	m_RenderSize		= size;
	m_RenderSizeMargin	= margin;

	for ( auto& it : m_ChunkActors ){
		it->Destroy();
	}

	FIntVector	vec;
	int32		count;

	vec		= size + margin * 2;
	count	= vec.X * vec.Y;
	m_ChunkActors.SetNum( count );

	for ( auto& it : m_ChunkActors ){
		it	= GetWorld()->SpawnActor<ANaMapChunkActor>();
		it->SetWorldActor( this );
#if WITH_EDITOR
		it->SetFolderPath( "/MapChunks" );
#endif							
	}

	if ( m_CaptureCube ){
		m_CaptureCube->ShowOnlyActors.Reset();
		for ( auto& it : m_ChunkActors ){
			m_CaptureCube->ShowOnlyActors.Add( it );
		}
		m_CaptureCube->PrimitiveRenderMode	= ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	}
}

//! チャンクアクター取得
ANaMapChunkActor* ANaWorldActor::GetChunkActor( const FIntVector& chunkPos )
{
	FIntVector	apos,vec;
	int32		tmp;

	vec		= m_RenderSize + m_RenderSizeMargin * 2;
	apos.X	= chunkPos.X % vec.X;
	apos.Y	= chunkPos.Y % vec.Y;
	apos.Z	= 0;

	if ( apos.X < 0 ){
		apos.X	+= vec.X;
	}
	if ( apos.Y < 0 ){
		apos.Y	+= vec.Y;
	}

	tmp	= apos.X + apos.Y * vec.X;

	return m_ChunkActors[tmp];
}

//! ブロックマテリアル取得
UMaterialInstanceDynamic* ANaWorldActor::FindBlockMaterial( int32 id )
{
	UMaterialInstanceDynamic*	retVal = m_MIDMap.FindRef( id );

	if ( !retVal ){
		UNaAssetLibrary*	alib = UNaAssetLibrary::Get();

		FNaBlockMaterialAsset*			asset = alib->FindBlockMaterialAsset( id );
		TAssetPtr<UMaterialInterface>	aptr( asset->Material );

		retVal	= UMaterialInstanceDynamic::Create( aptr.LoadSynchronous(), this );
		m_MIDMap.Add( id, retVal );
	}

	return retVal;
}

//! エンティティアクター生成
ANaActorBase* ANaWorldActor::SpawnEntityActor( UNaEntity* entity )
{
	UNaAssetLibrary*			alib = UNaAssetLibrary::Get();
	const FNaActorDataAsset*	asset = alib->FindActorAsset( entity->GetProfile()->ActorName );
	ANaActorBase*				actor = nullptr;

	if ( asset ){
		actor	= GetWorld()->SpawnActor<ANaActorBase>( asset->ActorClass );
		check( actor );

		actor->Initialize( entity->GetNaWorld(), entity );

		if ( entity == UNaGameDatabase::GetDB()->GetPlayer() ){
			m_Camera->AttachTarget( actor );
		}
	}

	return actor;
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//
void ANaWorldActor::ProcMain( UNaStateMachine* sm, float DeltaTime )
{
	enum EPhase
	{
		//! 
		Start,
		//! 
		Main,
		//! 
		End
	};

	switch ( sm->GetPhase() ){
	case Start:
		sm->Advance();
		break;

	case Main:
		//! ワールド更新
		for ( auto& it : m_Worlds ){
			it->Update( DeltaTime );
		}

		if ( m_ActiveWorld ){
			UNaEntityPlayer*	player;
			AActor*	actor;

			player	= m_ActiveWorld->GetPlayer();
			if ( player ){
				actor	= player->GetActor();

				//! マテリアルパラメータ設定
				for ( auto& it : m_MIDMap ){
					if ( actor ){
						it.Value->SetVectorParameterValue( "PlayerLocation", actor->GetActorLocation() );
					}
					it.Value->SetScalarParameterValue( "CeilZ", m_ActiveWorld->GetCeilZ() );
				}

				//! カメラアングル設定
				int32	dir = int32( m_ActiveWorld->GetWorldDirection() );
				m_Camera->SetAngle( dir * 45.0f );

				//! 視界表示更新
				{
					FIntVector	cpos;
					FVector		v;

					cpos	= m_ActiveWorld->GetCurrentPosition();
					if ( cpos != m_CurrentPos ){
						FNaWorldBlockWork	work;

						v.X		= cpos.X * 10.0f + 5.0f;
						v.Y		= cpos.Y * 10.0f + 5.0f;
						v.Z		= cpos.Z * 10.0f + 7.5f;

						m_ActiveWorld->GetBlock( cpos, work );
						v.Z		+= work.GetCenterHeight() / 256.0f * 10.0f;

						if ( m_CaptureCube ){
							m_CaptureCube->SetRelativeLocation( v );
							m_CaptureCube->CaptureSceneDeferred();
						}
						if ( m_MIDPost ){
							m_MIDPost->SetVectorParameterValue( "CapturePosition", v );
						}
						m_CurrentPos	= cpos;

						cpos.X	= cpos.X >> 4;
						cpos.Y	= cpos.Y >> 4;
						cpos.Z	= cpos.Z >> 4;

						if ( m_CurrentChunkPos != cpos ){
							m_CurrentChunkPos	= cpos;
							UpdateChunkActor();
						}
					}
				}
			}
		}

		//! 表示ワールド変更
		if ( m_NextWorld ){
			m_ActiveWorld	= m_NextWorld;
			m_NextWorld		= nullptr;
		}
		break;

	case End:
		break;
	}
}

//! チャンクアクター更新
void ANaWorldActor::UpdateChunkActor()
{
	FIntVector	chunkPos;

	chunkPos.Z	= m_CurrentChunkPos.Z - (m_RenderSize.Z >> 1);

	for ( int32 iz = 0; iz < 16; ++iz ){

		chunkPos.Z	= iz;
		chunkPos.Y	= m_CurrentChunkPos.Y - (m_RenderSize.Y >> 1);

		for ( int32 iy = 0; iy < m_RenderSize.Y; ++iy ){

			chunkPos.X	= m_CurrentChunkPos.X - (m_RenderSize.X >> 1);

			for ( int32 ix = 0; ix < m_RenderSize.X; ++ix ){
				UNaChunk*			chunk;
				ANaMapChunkActor*	actor;

				chunk	= m_ActiveWorld->GetChunk( chunkPos );
				if ( chunk ){
					actor	= GetChunkActor( chunkPos );

					if ( actor ){
						actor->SetChunk( chunk );
					}
				}

				chunkPos.X++;
			}

			chunkPos.Y++;
		}

		chunkPos.Z++;
	}
}
