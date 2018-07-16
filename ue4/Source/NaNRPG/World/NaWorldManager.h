// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Misc/NaEnums.h"

#include "Generation/NaWorldGenerator.h"

#include "NaWorldDefinition.h"
#include "NaWorld.h"

#include "NaWorldManager.generated.h"

class ANaWorldActor;
class UNaTurnActionComponent;
class UNaEventManager;

/**
 * �Q�[�����[���h�}�l�[�W��
 * �������[���h�̓����Ǘ��E�y�у����^�C���ŗL�����̎��s���s��
 */
UCLASS()
class NANRPG_API UNaWorldManager : public UObject
{
	GENERATED_BODY()

public:
	//! �R���X�g���N�^
	UNaWorldManager();

	//! ������
	void	Initialize( ANaWorldActor* actor );

	//! �X�V����
	void	Update( float DeltaTime );
	//! �A�N�V�����`�F�C���ɒǉ�
	void	InsertActionChain( UNaTurnActionComponent* tac );

	//! ���[���h�I�[�v��
	UNaWorld*	OpenWorld( FName id, FName assetID = FName() );
	//! ���[���h�N���[�Y
	void		CloseWorld( FName id );
	//! ���[���h�ύX���N�G�X�g
	void		ChangeWorld( FName id );

	//! ���[���h�W�F�l���[�^�擾
	UNaWorldGenerator*	GetGenerator() const	{ return m_Generator; }
	//! �A�N�e�B�u���[���h�擾
	UNaWorld*			GetActiveWorld() const	{ return m_ActiveWorld; }

protected:
	
protected:
	//! ���[���h�W�F�l���[�^
	UPROPERTY(Transient)
	UNaWorldGenerator*	m_Generator;
	//! �C�x���g�}�l�[�W��
	UPROPERTY()
	UNaEventManager*	m_EventManager;

	//! �I�[�v�����̃��[���h
	UPROPERTY()
	TArray<UNaWorld*>	m_Worlds;
	//! �A�N�e�B�u���[���h�i�\�����郏�[���h�j
	UPROPERTY()
	UNaWorld*			m_ActiveWorld;
	//! ���[���h�ύX���N�G�X�g
	UPROPERTY()
	UNaWorld*			m_RequestWorld;

	//! �A�N�V�������X�g
	UPROPERTY(Transient)
	TArray<UNaTurnActionComponent*>	m_ActionChain;
	//! �J�����g�A�N�V����
	UPROPERTY(Transient)
	UNaTurnActionComponent*			m_CurrentAction;

	//! ���[���h�\���Ǘ��A�N�^�[
	UPROPERTY(Transient)
	ANaWorldActor*		m_WorldActor;
	//! UE���[���h
	UPROPERTY(Transient)
	UWorld*				m_WorldContext;
};
