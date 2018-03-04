// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaWorldActor.h"

#include "Assets/NaAssetLibrary.h"

#include "Actor/Entity/NaActorBase.h"
#include "Actor/Entity/Character/NaCharacter.h"

#include "World/Map/NaMap.h"

#include "Entity/INaEntityFactory.h"
#include "Entity/Character/NaEntityPlayer.h"

#include "Database/NaGameDatabase.h"

ANaWorldActor::ANaWorldActor( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer )
{
 	// 
	PrimaryActorTick.bCanEverTick = true;

	//! 
	m_SM	= NewObject<UNaStateMachine>();
	if ( m_SM ){
		FNaStateDelegate	func;

		func.BindUObject( this, &ANaWorldActor::ProcMain );
		m_SM->RegisterState( EState::Main, func );
	}

	// ルートコンポーネント //
	UStaticMeshComponent*	comp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT("RootComponent") );
	RootComponent	= comp;

	// ライト //
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

	m_CurrentChunkPos.Z	= INT_MAX;
}

// 
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

	m_SM->ChangeState( EState::Main );
}
	
// 
void ANaWorldActor::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

	m_SM->Execute( DeltaSeconds );

	switch ( m_State ){
	case EState::Main:
		ProcMainOld( DeltaSeconds );
		break;
	}

	if ( m_World ){
		UNaEntityPlayer*	player;
		AActor*	actor;

		player	= m_World->GetPlayer();
		actor	= player->GetActor();

		for ( auto& it : m_MIDMap ){
			if ( actor ){
				it.Value->SetVectorParameterValue( "PlayerLocation", actor->GetActorLocation() );
			}
			it.Value->SetScalarParameterValue( "CeilZ", m_World->GetCeilZ() );
		}

		int32	dir = int32( m_World->GetWorldDirection() );
		m_Camera->SetOrbitalAngle( dir * 45.0f );

		FIntVector	cpos;
		FVector		v;

		cpos	= m_World->GetCurrentPosition();
		if ( cpos != m_CurrentPos ){
			FNaWorldBlockWork	work;

			v.X		= cpos.X * 10.0f + 5.0f;
			v.Y		= cpos.Y * 10.0f + 5.0f;
			v.Z		= cpos.Z * 10.0f + 7.5f;

			m_World->GetBlock( cpos, work );
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

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
// マップ読み込み
void ANaWorldActor::LoadMap(int32 mapID)
{
	UWorld*	const		world = GetWorld();
	APlayerController*	pc = world->GetFirstPlayerController();
	FIntVector	ppos( 0, 0, 64 );

	CloseWorld();

	if ( mapID != 3 ){
		m_World	= UNaWorld::Open( mapID );
	}
	if ( !m_World ){
		m_World	= UNaWorld::Create( mapID );
	}

	if ( m_World ){
		UNaGameDatabase*	db = UNaGameDatabase::GetDB();
		UNaEntity*			entity;

		m_World->Initialize( GetWorld() );
		if ( mapID == 2 ){
			m_World->SetChunkLimit( FIntVector(0, 0, -1), FIntVector(4, 4, 1) );
		}
		if ( mapID == 3 ){
			UNaMapAsset*	asset = NewObject<UNaMapAsset>();
			UNaMap*			map = NewObject<UNaMap>();

			asset->CreateSimpleRandomMap( FIntVector( 4, 4, 1 ) );
			map->Instantiate( asset );

//			m_World->EntryMap( FIntVector( -2, -2, 0 ), map );
		}
		m_World->SetCurrentPosition( ppos );
	
		// プレイヤー配置
		entity	= db->GetPlayer();
		entity->SetWorldPosition( ppos );
		m_World->SpawnEntity( entity, ppos );

		// 
//		entity	= INaEntityFactory::NewEntity( ENaEntity::GLOBAL_SPAWNER );
//		entity->SetStage( ENaEntityStage::Transient );
//		m_World->SpawnEntity( entity );
	}
}

//!
void ANaWorldActor::AssignWorld( UNaWorld* world )
{
	CloseWorld();

	m_World	= world;
}

// //
void ANaWorldActor::CloseWorld()
{
	if ( !m_World ){
		return;
	}

//	for ( auto& it : m_ChunkActors ){
//		it->Destroy();
//	}
//	m_ChunkActors.Reset();

	m_World->CloseWorld();
	m_World	= nullptr;
}

//
void ANaWorldActor::ChangeState(EState state, int32 param, bool immediate)
{
	m_State			= state;
	m_StateParam	= param;
	m_StateStep		= 0;
}

//! 
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
//	count	= vec.X * vec.Y * vec.Z;
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

//! 
void ANaWorldActor::UpdateChunkActor()
{
	FIntVector	chunkPos;

	chunkPos.Z	= m_CurrentChunkPos.Z - (m_RenderSize.Z >> 1);

//	for ( int32 iz = 0; iz < m_RenderSize.Z; ++iz ){
	for ( int32 iz = 0; iz < 16; ++iz ){

		chunkPos.Z	= iz;
		chunkPos.Y	= m_CurrentChunkPos.Y - (m_RenderSize.Y >> 1);

		for ( int32 iy = 0; iy < m_RenderSize.Y; ++iy ){

			chunkPos.X	= m_CurrentChunkPos.X - (m_RenderSize.X >> 1);

			for ( int32 ix = 0; ix < m_RenderSize.X; ++ix ){
				UNaChunk*			chunk;
				ANaMapChunkActor*	actor;

				chunk	= m_World->GetChunk( chunkPos );
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

//! 
ANaMapChunkActor* ANaWorldActor::GetChunkActor( const FIntVector& chunkPos )
{
	FIntVector	apos,vec;
	int32		tmp;

	vec		= m_RenderSize + m_RenderSizeMargin * 2;
	apos.X	= chunkPos.X % vec.X;
	apos.Y	= chunkPos.Y % vec.Y;
//	apos.Z	= chunkPos.Z % vec.Z;
	apos.Z	= 0;
	if ( apos.X < 0 ){
		apos.X	+= vec.X;
	}
	if ( apos.Y < 0 ){
		apos.Y	+= vec.Y;
	}
//	if ( apos.Z < 0 ){
//		apos.Z	+= vec.Z;
//	}

//	tmp	= apos.X + apos.Y * vec.X + apos.Z * vec.X * vec.Y;
	tmp	= apos.X + apos.Y * vec.X;

	return m_ChunkActors[tmp];
}

#if 0
//
void ANaWorldActor::UpdateMap( const FIntVector& pos )
{
	const FIntVector	c_DrawRange( 3, 3, 3 );
	FIntVector	cpos;

	cpos		= m_World->GetCurrentPosition();
	cpos.X	= cpos.X >> 4;
	cpos.Y	= cpos.Y >> 4;
	cpos.Z	= cpos.Z >> 4;

	if ( m_CurrentChunkPos != cpos ){
		TMap<FIntVector, ANaMapChunkActor*>	pool;
		TArray<UNaChunk*>	newChunks;
		FIntVector			tpos;
		ANaMapChunkActor*	chunkActor;
		UNaChunk*			chunk;

		pool	= m_ChunkActors;
		m_ChunkActors.Reset();

		tpos.X	= cpos.X - (c_DrawRange.X >> 1);

		for ( int32 ix = 0; ix < c_DrawRange.X; ++ix, ++tpos.X ){
			//
			tpos.Y	= cpos.Y - (c_DrawRange.Y >> 1);

			for ( int32 iy = 0; iy < c_DrawRange.Y; ++iy, ++tpos.Y ){
				//
				tpos.Z	= cpos.Z - (c_DrawRange.Z >> 1);

				for ( int32 iz = 0; iz < c_DrawRange.Z; ++iz, ++tpos.Z ){
					//
					chunkActor	= pool.FindRef( tpos );

					if ( chunkActor ){
						m_ChunkActors.Add( tpos, chunkActor );
						pool.Remove( tpos );
					}
					else {
						chunk	= m_World->GetChunk( tpos );
						if ( chunk ){
							newChunks.Add( chunk );
						}
					}
				}
			}
		}

		if ( newChunks.Num() ){
			UWorld*						world = GetWorld();
			TArray<ANaMapChunkActor*>	pools;

			pool.GenerateValueArray( pools );

			for ( int32 i = 0; i < newChunks.Num(); ++i ){
				if ( pools.Num() > 0 ){
					chunkActor	= pools.Pop();
				}
				else {
					chunkActor	= world->SpawnActor<ANaMapChunkActor>();
					chunkActor->SetWorldActor( this );
				}

				if ( chunkActor ){
					chunkActor->SetChunk( newChunks[i] );
//					chunkActor->UpdateChunkMesh();
#if WITH_EDITOR
					chunkActor->SetFolderPath( "/MapChunks" );
#endif							
					m_ChunkActors.Add( newChunks[i]->GetPositionInWorld(), chunkActor );
				}
			}
		}

		m_CurrentChunkPos	= cpos;
	}

	{// 天井再計算 //
#if 0
		int32	cz = m_World->GetCeilZ();

		if ( m_CurrentZ != cz ){
			TArray<ANaMapChunkActor*>	list;
			
			m_ChunkActors.GenerateValueArray( list );
			for ( auto& it : list ){
				it->UpdateChunkMesh();
			}
			m_CurrentZ	= cz;
		}
		if ( cz >= 64 ){
			m_pDirLight->SetVisibility( true );
		}
		else {
			m_pDirLight->SetVisibility( false );
		}
#endif
	}
}
#endif

//
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

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//
bool ANaWorldActor::ProcMain( UNaStateMachine* sm, float DeltaTime )
{
	return true;
}

//
void ANaWorldActor::ProcMainOld(float DeltaTime)
{
	enum StateStep
	{
		Start,
		Main,
		End
	};
	UWorld*				world = GetWorld();
	APlayerController*	pc = world->GetFirstPlayerController();

	switch ( m_StateStep ){
	case Start:
/*		for ( auto it : m_WorldActors ){
			m_ActionActors.Add( it );
		}

		m_ActionActors.StableSort( []( const ANaActorBase& lhs, const ANaActorBase& rhs )
		{
			return lhs.GetWaitTime() < rhs.GetWaitTime();
		});
		*/
		m_StateStep++;
		break;

	case Main:
		//
		m_World->AdvanceTurn();

/*		while ( true ){
			if ( !m_pCurrentActor ){
				m_pCurrentActor	= m_ActionActors[0];
				m_ActionActors.RemoveAt( 0 );

				for ( auto it : m_ActionActors ){
					it->DecreaseWaitTime( m_pCurrentActor->GetWaitTime() );
				}
				m_pCurrentActor->StartTurn();
			}

			m_pCurrentActor->ExecuteTurn();

			if ( m_pCurrentActor->IsEndTurn() ){
				int32	i;

				if ( m_pCurrentActor == m_Player ){
					UpdateMap( m_Player->GetWorldPosition() );
				}

				m_pCurrentActor->ResetWaitTime();
				for ( i = 0; i < m_ActionActors.Num(); ++i ){
					if ( m_pCurrentActor->GetWaitTime() < m_ActionActors[i]->GetWaitTime() ){
						break;
					}
				}
				m_ActionActors.Insert( m_pCurrentActor, i );

				m_pCurrentActor	= nullptr;
				continue;
			}

			break;
		}*/
		break;

	case End:
		break;
	}
}
