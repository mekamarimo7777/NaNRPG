// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"

#include "Actor/World/NaWorldActor.h"
#include "Actor/Camera/NaCameraActor.h"

#include "World/NaWorld.h"

#include "Utility/Components/NaStateMachine.h"

#include "NaMainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NANRPG_API ANaMainGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	//! �X�e�[�g
	enum EState
	{
		Main,
	};

public:
	//! �R���X�g���N�^
	ANaMainGameMode( const FObjectInitializer& ObjectInitializer );

	//! �J�n����
	virtual void	BeginPlay() override;
	//! �I������
	virtual void	EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//! �X�V
	virtual void	Tick(float DeltaTime) override;

public:
	//
	void		TravelToWorld(int32 worldID);
	//
	UNaWorld*	LoadWorld( int32 uid, FName worldID );

	//
	ANaCameraActor*	GetCamera() const	{ return m_Camera; }

protected:
	//! ���C��
	void	ProcMain( UNaStateMachine* sm, float DeltaTime );

	//! ���[���h�Ǘ��A�N�^�[����
	void	CreateWorldActor();

public:
	//! �J�����A�N�^�[�N���X
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
	TSubclassOf<ANaCameraActor>		CameraActorClass;
	//! ���[���h�Ǘ��A�N�^�[�N���X
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "World Actor" )
	TSubclassOf<ANaWorldActor>		WorldActorClass;

protected:
	//! ��ԊǗ�
	UPROPERTY()
	UNaStateMachine*	m_SM;

	//! ���[���h�A�N�^�[
	UPROPERTY()
	ANaWorldActor*		m_WorldActor;
	//! �J����
	UPROPERTY()
	ANaCameraActor*		m_Camera;
};
