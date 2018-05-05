// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaCharacter.h"

#include "Animation/AnimInstance.h"

#include "Assets/NaAssetLibrary.h"
#include "Database/NaGameDatabase.h"

#include "GameMode/NaMainGameMode.h"

// Sets default values
ANaCharacter::ANaCharacter(const FObjectInitializer& ObjectInitializer)
{
	// 
	m_Capsule		= CreateDefaultSubobject<UCapsuleComponent>( TEXT("Capsule") );
	RootComponent	= m_Capsule;

	m_StaticMesh	= CreateOptionalDefaultSubobject<UStaticMeshComponent>( TEXT("StaticMesh0") );
	m_StaticMesh->AttachToComponent( m_Capsule, FAttachmentTransformRules::KeepRelativeTransform );

	m_SkeletalMesh	= CreateOptionalDefaultSubobject<USkeletalMeshComponent>( TEXT("SkeletalMesh0") );
	m_SkeletalMesh->CastShadow	= false;
	m_SkeletalMesh->AttachToComponent( m_Capsule, FAttachmentTransformRules::KeepRelativeTransform );
}

// Called when the game starts or when spawned
void ANaCharacter::BeginPlay()
{
	Super::BeginPlay();

	ChangeState( EState::Waiting );
}

// Called every frame
void ANaCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	switch ( m_State ){
	case EState::Waiting:
		ProcWaiting( DeltaTime );
		break;
	case EState::Action:
		ProcAction( DeltaTime );
		break;
	case EState::Attack:
		ProcAttack( DeltaTime );
		break;
	case EState::Death:
		ProcDeath( DeltaTime );
		break;
	}
}

//! アイテム情報取得
UNaItem* ANaCharacter::GetItemProperty() const
{
	return m_pEntity->GetItemProperty();
}

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//
bool ANaCharacter::SetAnimation( ENaActorAnimation::Type anim )
{
	if ( m_ReqAnim == anim ){
		return true;
	}

	m_ReqAnim	= anim;

	if ( m_SkeletalMesh && m_SkeletalMesh->GetAnimInstance() && Animations[anim] ){
		if ( Animations[m_ReqAnim] ){
			m_SkeletalMesh->GetAnimInstance()->Montage_Stop( 0.5f, Animations[m_ReqAnim] );
		}
		m_SkeletalMesh->GetAnimInstance()->Montage_Play( Animations[anim] );

		return true;
	}

	return false;
}

//
bool ANaCharacter::IsAnimationPlaying() const
{
	if ( m_SkeletalMesh && m_SkeletalMesh->GetAnimInstance() ){
		return m_SkeletalMesh->GetAnimInstance()->Montage_IsPlaying( Animations[m_ReqAnim] );
	}
	else {
		return false;
	}
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//
void ANaCharacter::OnInitialize()
{
	UNaAssetLibrary*		alib = UNaAssetLibrary::Get();
	const FNaEntityProfile*	prof = m_pEntity->GetProfile();
	FNaActorDataAsset*		asset;

	asset	= alib->FindActorAsset( prof->ActorName );
	if ( asset ){
		// skeletal mesh
		if ( !asset->SkeletalMesh.IsNull() ){
			if ( !asset->SkeletalMesh.IsValid() ){
				// ひとまず同期ロード
				asset->SkeletalMesh.LoadSynchronous();
			}
			m_SkeletalMesh->SetSkeletalMesh( asset->SkeletalMesh.Get() );
		}

		// skeletal mesh
		if ( !asset->StaticMesh.IsNull() ){
			if ( !asset->StaticMesh.IsValid() ){
				// ひとまず同期ロード
				asset->StaticMesh.LoadSynchronous();
			}
			m_StaticMesh->SetStaticMesh( asset->StaticMesh.Get() );
		}

		m_Capsule->SetWorldScale3D( asset->Scale );
	}
	
	SetWorldPosition( m_pEntity->GetWorldPosition(), true );

//	m_Capture[0]->TextureTarget			= m_pWorld->DataAsset->RenderTarget;
//	m_Capture[0]->AddOrUpdateBlendable( m_pWorld->DataAsset->RenderTargetShader );
}

//
void ANaCharacter::ProcWaiting(float DeltaTime)
{
	enum StateStep
	{
		Start,
		Idle,
		Walk,
		End
	};

	switch ( m_StateStep ){
	case Start:
		SetAnimation( ENaActorAnimation::Idle );
		m_StateStep++;
		break;
	case Idle:
		if ( m_bKill ){
			ChangeState( EState::Death );
			break;
		}
		if ( IsMoving() ){
			SetAnimation( ENaActorAnimation::Walk );
			m_StateStep		= Walk;
		}
		break;
	case Walk:
		if ( IsMoving() ){
			m_StateParam	= 10;
		}
		else {
			if ( --m_StateParam < 0 ){
				SetAnimation( ENaActorAnimation::Idle );
				m_StateStep		= Idle;
			}
		}
		break;
	case End:
		break;
	}
}

//
void ANaCharacter::ProcAction(float DeltaTime)
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
		m_StateStep++;
		break;
	case End:
		ChangeState( EState::Waiting );
		break;
	}
}

//
void ANaCharacter::ProcAttack(float DeltaTime)
{
	enum StateStep
	{
		Start,
		Main,
		End
	};
	UWorld*	world = GetWorld();

	switch ( m_StateStep ){
	case Start:
		{
			const FVector	c_dirPos[8] = 
			{
				FVector(  0.0f, -1.0f, 0.0f ),
				FVector(  1.0f, -1.0f, 0.0f ),
				FVector(  1.0f,  0.0f, 0.0f ),
				FVector(  1.0f,  1.0f, 0.0f ),
				FVector(  0.0f,  1.0f, 0.0f ),
				FVector( -1.0f,  1.0f, 0.0f ),
				FVector( -1.0f,  0.0f, 0.0f ),
				FVector( -1.0f, -1.0f, 0.0f ),
			};
			FVector	pos;

			pos	= GetActorLocation();
			pos	+= c_dirPos[int32(m_pEntity->GetDirection())] * 10.0f;

//			m_pParticle	= UGameplayStatics::SpawnEmitterAtLocation( world, m_pWorld->DataAsset->TestAttackParticle, pos );
//			m_pParticle->SetWorldScale3D( FVector( 0.5f, 0.5f, 0.5f ) );

			SetAnimation( ENaActorAnimation::Attack );

//			m_StateParam	= 30;
			m_StateStep++;
		}
		break;
	case Main:
		if ( !IsAnimationPlaying() ){
			if ( m_pParticle ){
				m_pParticle->DestroyComponent();
				m_pParticle	= nullptr;
			}

			m_StateStep++;
		}
		break;
	case End:
		ChangeState( EState::Waiting );
		break;
	}
}

//
void ANaCharacter::ProcDeath(float DeltaTime)
{
	enum StateStep
	{
		Start,
		Main,
		End
	};
	UWorld*	world = GetWorld();

	switch ( m_StateStep ){
	case Start:
		if ( SetAnimation( ENaActorAnimation::KnockDown ) ){
			m_StateStep++;
		}
		else {
			m_StateStep	= End;
		}
		break;
	case Main:
		if ( !IsAnimationPlaying() ){
			m_StateStep++;
		}
		break;
	case End:
		Destroy();
		break;
	}
}
