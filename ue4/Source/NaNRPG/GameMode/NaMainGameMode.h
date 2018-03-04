// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"

#include "Actor/World/NaWorldActor.h"
#include "Actor/Camera/NaCameraActor.h"

#include "World/NaWorld.h"

#include "NaMainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NANRPG_API ANaMainGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	//! �R���X�g���N�^
	ANaMainGameMode( const FObjectInitializer& ObjectInitializer );

	// 
	virtual void	BeginPlay() override;
	// 
	virtual void	EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 
	virtual void	Tick(float DeltaTime) override;

public:
	//
	void	ChangeState(int32 state, int32 param = 0, bool immediate = false);

	//
	void		TravelToWorld(int32 worldID);
	//
	UNaWorld*	LoadWorld( int32 uid, FName worldID );

	//
	ANaCameraActor*	GetCamera() const	{return m_pCamera;}

protected:
	//
	void	ProcMain(float DeltaTime);

public:
	//! �J�����A�N�^�[�N���X
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
	TSubclassOf<ANaCameraActor>		CameraActorClass;
	//! ���[���h�Ǘ��A�N�^�[�N���X
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "World Actor" )
	TSubclassOf<ANaWorldActor>		WorldActorClass;

protected:
	//
	int32	m_State;
	int32	m_StateParam;
	int32	m_StateStep;

	//!
	UPROPERTY()
	UNaWorld*		m_NaWorld;

	// �}�b�v //
	UPROPERTY()
	ANaWorldActor*		m_pMapActor;
	// �J���� //
	UPROPERTY()
	ANaCameraActor*		m_pCamera;
};
