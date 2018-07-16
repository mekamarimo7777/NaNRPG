// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaCharacter.h"

#include "Animation/AnimInstance.h"

#include "Assets/NaAssetLibrary.h"
#include "Database/NaGameDatabase.h"

#include "GameMode/NaMainGameMode.h"

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
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

	if ( m_SM ){
		m_SM->RegisterState( EState::Waiting, this, &ANaCharacter::ProcWaiting );
		m_SM->RegisterState( EState::Attack, this, &ANaCharacter::ProcAttack );
		m_SM->RegisterState( EState::Death, this, &ANaCharacter::ProcDeath );
	}
	m_SM->ChangeState( EState::Waiting );
}

// Called every frame
void ANaCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

//! アイテム情報取得
UNaItem* ANaCharacter::GetItemProperty() const
{
	return m_Entity->GetItemProperty();
}

//! アクション要求
void ANaCharacter::RequestAction( FName action )
{
	if ( action == "Attack" ){
		m_SM->ChangeState( EState::Attack );
	}
}

//! アクション中判定
bool ANaCharacter::IsAction() const
{
	return m_SM->GetState() != EState::Waiting;
}

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
	const FNaEntityProfile*	prof = m_Entity->GetProfile();
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
	
	SetWorldPosition( m_Entity->GetWorldPosition(), true );
}

//! 待機中
void ANaCharacter::ProcWaiting( UNaStateMachine* sm, float DeltaTime )
{
	enum StateStep
	{
		Start,
		Idle,
		Walk,
		End
	};

	switch ( sm->GetPhase() ){
	case Start:
		SetAnimation( ENaActorAnimation::Idle );
		sm->Advance();
		break;
	case Idle:
		if ( !m_Entity->IsAlive() ){
			sm->ChangeState( EState::Death );
			break;
		}
		if ( IsMoving() ){
			SetAnimation( ENaActorAnimation::Walk );
			sm->SetPhase( StateStep::Walk );
		}
		break;
	case Walk:
		if ( IsMoving() ){
			m_Param	= 10;
		}
		else {
			if ( --m_Param < 0 ){
				SetAnimation( ENaActorAnimation::Idle );
				sm->SetPhase( StateStep::Idle );
			}
		}
		break;
	case End:
		break;
	}
}

//! 攻撃中
void ANaCharacter::ProcAttack( UNaStateMachine* sm, float DeltaTime )
{
	enum StateStep
	{
		Start,
		Main,
		End
	};
	UWorld*	world = GetWorld();

	switch ( sm->GetPhase() ){
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
			pos	+= c_dirPos[int32(m_Entity->GetDirection())] * 10.0f;

//			m_pParticle	= UGameplayStatics::SpawnEmitterAtLocation( world, m_pWorld->DataAsset->TestAttackParticle, pos );
//			m_pParticle->SetWorldScale3D( FVector( 0.5f, 0.5f, 0.5f ) );

			SetAnimation( ENaActorAnimation::Attack );

//			m_StateParam	= 30;
			sm->Advance();
		}
		break;
	case Main:
		if ( !IsAnimationPlaying() ){
			if ( m_pParticle ){
				m_pParticle->DestroyComponent();
				m_pParticle	= nullptr;
			}
			sm->Advance();
		}
		break;
	case End:
		sm->ChangeState( EState::Waiting );
		break;
	}
}

//! 死亡中
void ANaCharacter::ProcDeath( UNaStateMachine* sm, float DeltaTime )
{
	enum StateStep
	{
		Start,
		Main,
		End
	};
	UWorld*	world = GetWorld();

	switch ( sm->GetPhase() ){
	case Start:
		if ( SetAnimation( ENaActorAnimation::KnockDown ) ){
			sm->Advance();
		}
		else {
			sm->SetPhase( StateStep::End );
		}
		break;
	case Main:
		if ( !IsAnimationPlaying() ){
			sm->Advance();
		}
		break;
	case End:
		Kill();
		break;
	}
}
