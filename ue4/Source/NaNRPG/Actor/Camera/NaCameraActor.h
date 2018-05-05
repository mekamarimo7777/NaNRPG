// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraActor.h"

#include "NaCameraActor.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class NANRPG_API ANaCameraActor : public ACameraActor
{
	GENERATED_BODY()
	
public:
	//! �R���X�g���N�^
	ANaCameraActor( const FObjectInitializer& ObjectInitializer );

	//! �J�n
	virtual void BeginPlay() override;

	//! �X�V����
	virtual void Update( float DeltaSeconds );

public:
	//! �o�C���h��ݒ�
	void	AttachTarget( AActor* target );
	//! �A���O���ݒ�
	void	SetAngle( float angle );

protected:
	//! �J�����p�x�ݒ�
	void	ApplyCameraAngle();

public:
	//! �J�����s�b�`
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraActor)
	float	OrbitalPitch;
	//! �J��������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraActor)
	float	OrbitalRange;

protected:
	//! �����^�[�Q�b�g
	UPROPERTY()
	AActor*	m_pTarget;

	//! 
	FVector	m_CurrentPos;
	//! �J�����g�p�x
	float	m_CurrentAngle;
	//! ���N�G�X�g�p�x
	float	m_RequestAngle;
};
