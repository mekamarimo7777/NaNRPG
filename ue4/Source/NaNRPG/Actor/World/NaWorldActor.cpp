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

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//! 
ANaWorldActor::ANaWorldActor( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer )
{
 	//! 
	PrimaryActorTick.bCanEverTick = true;

	//! ���[�g�R���|�[�l���g
	UStaticMeshComponent*	comp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT("RootComponent") );
	RootComponent	= comp;

	//! ���C�g
	m_pDirLight	= CreateDefaultSubobject<UDirectionalLightComponent>( TEXT("DiretionalLight") );
	m_pDirLight->SetRelativeRotation( FRotator( -31.0f, 36.0f, -51.0f ) );
	m_pDirLight->AttachToComponent( RootComponent, FAttachmentTransformRules::KeepRelativeTransform );

	//! �Օ��L���v�`��
	m_CaptureCube	= CreateDefaultSubobject<USceneCaptureComponentCube>( TEXT("OccludeCapture") );
	if ( m_CaptureCube ){
		m_CaptureCube->AttachToComponent( RootComponent, FAttachmentTransformRules::KeepRelativeTransform );
		m_CaptureCube->CaptureSource		= ESceneCaptureSource::SCS_SceneDepth;
		m_CaptureCube->bCaptureEveryFrame	= false;
		m_CaptureCube->bCaptureOnMovement	= false;
	}
	//! �|�X�g�v���Z�X
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

	//! �X�e�[�g�Ǘ�
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

//! ���[���h�I�[�v��
UNaWorld* ANaWorldActor::OpenWorld( FName id, FName assetID )
{
	UNaGameDatabase*	db = UNaGameDatabase::GetDB();
	UWorld*	const		world = GetWorld();
	UNaWorld*			naw = nullptr;

	//! �G���g���₢���킹
	if ( FNaWorldRecord* rec = db->FindWorldEntry( id ) ){
		naw	= UNaWorld::Open( rec->DataID );
	}
	else {
		//! �V�K����
		naw	= UNaWorld::Create( id, assetID );
		check( naw );

		db->RegisterWorldEntry( id, naw->GetDataID() );
	}

	naw->Setup( world );

	m_Worlds.Add( naw );

	if ( !m_ActiveWorld ){
		m_NextWorld	= naw;
	}

	return naw;
}

//! ���[���h�N���[�Y
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

//! �A�N�e�B�u���[���h�ύX���N�G�X�g
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

//! �\���`�����N�͈͐ݒ�
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

//! �`�����N�A�N�^�[�擾
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

//! �u���b�N�}�e���A���擾
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
		//! ���[���h�X�V
		for ( auto& it : m_Worlds ){
			it->Update( DeltaTime );
		}

		if ( m_ActiveWorld ){
			UNaEntityPlayer*	player;
			AActor*	actor;

			player	= m_ActiveWorld->GetPlayer();
			if ( player ){
				actor	= player->GetActor();

				//! �}�e���A���p�����[�^�ݒ�
				for ( auto& it : m_MIDMap ){
					if ( actor ){
						it.Value->SetVectorParameterValue( "PlayerLocation", actor->GetActorLocation() );
					}
					it.Value->SetScalarParameterValue( "CeilZ", m_ActiveWorld->GetCeilZ() );
				}

				//! �J�����A���O���ݒ�
				int32	dir = int32( m_ActiveWorld->GetWorldDirection() );
				m_Camera->SetAngle( dir * 45.0f );

				//! ���E�\���X�V
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

		//! �\�����[���h�ύX
		if ( m_NextWorld ){
			m_ActiveWorld	= m_NextWorld;
			m_NextWorld		= nullptr;
		}
		break;

	case End:
		break;
	}
}

//! �`�����N�A�N�^�[�X�V
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
