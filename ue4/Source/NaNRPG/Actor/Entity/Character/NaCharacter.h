// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"

#include "Actor/Entity/NaActorBase.h"
#include "Actor/Camera/NaCameraActor.h"

#include "NaCharacter.generated.h"

class UNaItem;

UCLASS()
class NANRPG_API ANaCharacter : public ANaActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANaCharacter(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//! アイテム情報取得
	UFUNCTION( BlueprintCallable, Category = "EntityActor" )
	UNaItem*	GetItemProperty() const;
	
public:
	//
	bool	SetAnimation( ENaActorAnimation::Type anim );
	//
	bool	IsAnimationPlaying() const;

protected:
	//
	virtual void	OnInitialize() override;

	//
	void	ProcWaiting(float DeltaTime);
	//
	void	ProcAction(float DeltaTime);
	//
	void	ProcAttack(float DeltaTime);
	//
	void	ProcDeath(float DeltaTime);

	//
	void	CreateActor( FName assetID );

public:
	//! アニメーション
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage*			Animations[ENaActorAnimation::ENUM_MAX];

protected:
	//! コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model)
	UCapsuleComponent*		m_Capsule;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model)
	UStaticMeshComponent*	m_StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model)
	USkeletalMeshComponent*	m_SkeletalMesh;

	//!
	ENaActorAnimation::Type	m_ReqAnim;

	//
	UPROPERTY()
	ANaCharacter*	m_pLookTarget;

	UPROPERTY()
	UParticleSystemComponent*	m_pParticle;
};
