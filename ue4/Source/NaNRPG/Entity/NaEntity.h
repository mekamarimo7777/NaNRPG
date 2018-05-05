// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Misc/NaEnums.h"

#include "NaEntityConst.h"

#include "Assets/Entity/NaEntityDataTable.h"
#include "Assets/Event/NaEventAsset.h"

#include "Entity/Controller/NaEntityManipulator.h"
#include "Entity/Components/NaTurnActionComponent.h"

#include "Utility/Components/NaStateMachine.h"

#include "NaEntity.generated.h"

class UNaWorld;
class UNaChunk;

class UNaInventory;

struct FNaEntityDataAsset;

class ANaGameHUD;

/**
 * �G���e�B�e�B���
 */
UCLASS(abstract)
class NANRPG_API UNaEntity : public UObject
{
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	UNaEntity();

	// ������
	void	Initialize( ENaEntity::Type type );

	// ID�ݒ�
	void	SetID( int32 id );
	// ID����
	void	ResetID();

	// �X�|�[�������i�`�����N�f�[�^�ւ̒ǉ��j
	void	Spawn();
	// �f�X�|�[�������i�`�����N�f�[�^����폜�j
	void	Despawn();
	// ���ꏈ���i�`�����N�N�����̏����j
	void	Enter();
	// �ޏꏈ���i�`�����N�������̑ޔ������j
	void	Leave();

	// Na���[���h�ݒ�
	void		SetNaWorld( UNaWorld* world )	{ m_pWorld = world; }
	// Na���[���h�擾
	UNaWorld*	GetNaWorld() const				{ return m_pWorld; }

	//! �������[���hID�ݒ�
	void		SetWorldID( FName id )			{ m_WorldID = id; }
	//! �������[���hID�擾
	FName		GetWorldID() const				{ return m_WorldID; }
	//! �����}�b�vID�ݒ�
	void		SetMapID( int32 id )			{ m_MapID = id; }
	//! �����}�b�vID�擾
	int32		GetMapID() const				{ return m_MapID; }

	// UE���[���h�擾
	UWorld*		GetWorldContext() const			{ return m_pWorld ? m_pWorld->GetWorldContext() : nullptr; }
	// UEHUD�擾
	ANaGameHUD*	GetHUD() const					{ return Cast<ANaGameHUD>( GWorld->GetFirstPlayerController()->GetHUD() ); }

	//! �G���e�B�e�B��񐶐�
	virtual void	CreateFromAsset( const FNaEntityDataAsset& asset );
	//! �G���e�B�e�B�p�����[�^�ݒ�
	virtual void	SetEntityParameter( const TMap<FName, FString>& values );

	//!
	FNaEntityProfile*	GetProfile()		{return &m_Profile;}
	//!
	FString				GetDisplayName()	{return m_Profile.DisplayName;}

	//! �A�C�e�����Z�b�g
	virtual void		SetItemProperty( UNaItem* item )	{}
	//! �A�C�e�����擾
	virtual UNaItem*	GetItemProperty()					{ return nullptr; }

	//! �X�|�[������
	bool	IsSpawned() const	{ return m_Spawned; }
	//! ��������
	bool	IsAlive() const		{ return !m_IsKill; }

	//! �C�x���g�Z�b�g
	void	SetEvent( FName eventID );
	//! �C�x���g�擾
	bool	FindEvent( ENaEventTrigger trigger, const UNaEventAsset*& outEvent, int32& outSheet );






	// ���W�ݒ�
	virtual void	SetWorldPosition(const FIntVector& pos);
	virtual void	MoveToWorldPosition(const FIntVector& pos);
	void			UpdateCurrentChunk();
	//! ���[���h���W�擾
	FIntVector		GetWorldPosition() const				{ return m_WorldPos; }
	//! �����`�����N���W�擾
	FIntVector		GetChunkPosition() const;

	// �����ݒ�
	void			SetDirection( ENaDirection value )		{m_Direction = value;}
	ENaDirection	GetDirection() const					{return m_Direction;}
	// 
	void		SetSpeed(int32 value)		{m_Speed = value;}
	int32		GetSpeed() const			{return m_Speed;}
	//
	virtual void	TakeDamage( int32 value, UNaEntity* actor )	{}

	//
	int32	GetGroup() const	{return m_Group;}

	//
	ENaEntity::Type	GetType() const	{return m_Type;}

	//
	bool	FindMovePosition( ENaDirection dir, FIntVector& outPos );

	//==================================================
	// �X�e�[�g
	//==================================================
	//!
	virtual UNaStateMachine*	GetStateMachine() const			{ return m_SM; }

	//==================================================
	// �^�[���A�N�V����
	//==================================================
	//!
	virtual UNaTurnActionComponent*	GetTurnAction() const	{ return nullptr; }
	//!
	FORCEINLINE bool	HasTurnAction() const	{ return GetTurnAction() != nullptr; }

	// �폜
	void	Kill()	{ m_IsKill = true; }
	// �폜�҂�
	virtual bool	IsPendingKill() const	{ return m_IsKill; }

	//!
	virtual UNaEntityManipulator*	GetManipulator() const			{ return nullptr; }

	//
	void					SetStage( ENaEntityStage::Type stage )	{m_Stage = stage;}
	//
	ENaEntityStage::Type	GetStage() const	{return m_Stage;}

	//! ���`�G���e�B�e�B�i�`�����N�ɏ������Ȃ��j
	virtual bool	IsIntangible() const	{ return m_IsAbstract; }
	// �R���W�����̗L��
	virtual bool	IsCollidable() const	{ return !m_IsAbstract && m_Collidable; }

	// 
	//virtual bool	IsAbstract() const		{return m_IsAbstract;}
	// 
	//virtual bool	IsStationaly() const	{return m_IsStationary;}
	// 
	//virtual bool	IsNoSave() const		{return m_bNoSave;}

	// �V���A���C�Y
	virtual	void	Serialize( FArchive& ar ) override;

protected:
	// ������̏�����
	virtual void	OnAwake(){}
	// �폜�O
	virtual void	OnDestroy(){}
	// �X�|�[���㏈��
	virtual void	OnSpawn(){}
	// �f�X�|�[���O����
	virtual void	OnDespawn(){}
	// 
	virtual void	OnEnter(){}
	// 
	virtual void	OnLeave(){}

	//! �V���A���C�Y����
	virtual void	OnSerialize( FArchive& ar );

	//! �ꎞ���̍X�V
	void	UpdateTransientData( const FNaEntityDataAsset* asset );

public:
	//! �A�N�V�������Ȃ��i�A�N�V�����`�F�C���Ɍq���Ȃ��j
	bool	m_IsStationary;
	//! ���W�Ǘ����Ȃ��G���e�B�e�B�i�`�����N�ɑ����Ȃ��j
	bool	m_IsAbstract;
	//! �L���t���O
	bool	m_IsKill;
	
protected:
	//*** Serialize ***//
	//! �G���e�B�e�B���
	TEnumAsByte<ENaEntity::Type>	m_Type;
	//! �A�Z�b�gID
	FName							m_AssetID;
	//! �ŗLID
	uint32							m_ID;
	//! �G���e�B�e�B���
	FNaEntityProfile				m_Profile;

	//! �������x��
	TEnumAsByte<ENaEntityStage::Type>	m_Stage;
	//! �������[���hID�iWorld�X�e�[�W�ȏ�ɐݒ�j
	FName								m_WorldID;
	//! �����}�b�vID�i-1�Ŗ������j
	int32								m_MapID;
	//! 
	bool								m_Spawned;

	FIntVector		m_WorldPos;		// ���[���h���W //
	ENaDirection	m_Direction;	// ���� //

	//
	int32	m_Speed;			// �s�����x //
	int32	m_WaitTime;			// �҂����� //
	uint32	m_TotalTurn;		// �݌v�^�[�� //

	int32	m_Group;		// �������́i���j //

	//
	int32	m_LastUpdateTime;	// �O��̍s������ //
	int32	m_RevivalTime;		// �����҂����ԁi��������L�����̂݁j //

	//! �C�x���g
	FName					m_EventID;
	//! �G���e�B�e�B�p�����[�^
	TMap<FName, FString>	m_Params;

	//*** Serialize End ***//

protected:
	// �ꎞ�p�����[�^ //
	//! �������[���h�Q��
	UPROPERTY(Transient)
	UNaWorld*	m_pWorld;
	//! �����`�����N�Q��
	UPROPERTY(Transient)
	UNaChunk*	m_pCurrentChunk;

	//! �X�e�[�g�Ǘ�
	UPROPERTY(Transient)
	UNaStateMachine*	m_SM;

	//! �\�[�X�A�Z�b�g
	const FNaEntityDataAsset*	m_Asset;
	//! �T�C�Y
	FIntVector					m_Size;
	//! �R���W�����L��
	bool						m_Collidable;
};
