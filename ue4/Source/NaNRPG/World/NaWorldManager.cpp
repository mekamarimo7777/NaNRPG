// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaWorldManager.h"

#include "Actor/World/NaWorldActor.h"
#include "Actor/Entity/NaActorBase.h"

#include "Assets/NaAssetLibrary.h"
#include "Assets/World/NaBlockDataTable.h"

#include "Entity/INaEntityFactory.h"

#include "World/Event/NaEventManager.h"

#include "Database/NaGameDatabase.h"

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//! �R���X�g���N�^
UNaWorldManager::UNaWorldManager()
: m_CurrentAction( nullptr )
, m_EventManager( nullptr )
, m_WorldActor( nullptr )
, m_WorldContext( nullptr )
{
	m_Generator	= NewObject<UNaWorldGenerator>();
}

//! ������
void UNaWorldManager::Initialize( ANaWorldActor* actor )
{
	m_WorldActor	= actor;
	m_WorldContext	= m_WorldActor->GetWorld();

	m_EventManager	= NewObject<UNaEventManager>();
	//m_EventManager->Initialize( this );
}

//! �X�V����
void UNaWorldManager::Update( float DeltaTime )
{
	//! �^�[���A�N�V����
	while ( true ){
		// �폜�҂��G���e�B�e�B�폜
/*		for ( int32 i = m_SpawnEntities.Num() - 1; i >= 0; --i ){
			if ( m_SpawnEntities[i]->IsPendingKill() ){
				DespawnEntity( m_SpawnEntities[i] );
			}
		}*/

		// �^�[���A�N�V�����J�n
		if ( !m_CurrentAction && m_ActionChain.Num() > 0 ){
			m_CurrentAction	= m_ActionChain[0];
			m_ActionChain.RemoveAt( 0 );

			for ( auto it : m_ActionChain ){
				it->DecreaseWaitTime( m_CurrentAction->GetWaitTime() );
			}
			m_CurrentAction->BeginTurn();
		}

		// �^�[������
		if ( m_CurrentAction ){
			m_CurrentAction->ExecuteTurn( 0.0f );

			// �I�����Ȃ������ꍇ�͎��t���[����
			if ( !m_CurrentAction->IsEndTurn() ){
				break;
			}

			InsertActionChain( m_CurrentAction );
			m_CurrentAction	= nullptr;
		}
		else {
			break;
		}
	}

	//! �Q�[���C�x���g����
	if ( m_EventManager ){
		m_EventManager->Tick( DeltaTime );
	}
}

//! �A�N�V�����`�F�C���ɒǉ�
void UNaWorldManager::InsertActionChain( UNaTurnActionComponent* tac )
{
	int32	idx;

	tac->ResetWaitTime();

	for ( idx = 0; idx < m_ActionChain.Num(); ++idx ){
		if ( tac->GetWaitTime() < m_ActionChain[idx]->GetWaitTime() ){
			break;
		}
	}

	m_ActionChain.Insert( tac, idx );
}


//! ���[���h�I�[�v��
UNaWorld* UNaWorldManager::OpenWorld( FName id, FName assetID )
{
	UNaGameDatabase*	db = UNaGameDatabase::GetDB();
	UNaWorld*	naw;
	int32		idx;

	idx	= m_Worlds.IndexOfByPredicate( [id]( UNaWorld* p )
	{
		return p->GetUID() == id;
	});

	//! ���łɊJ���Ă���ꍇ
	if ( idx >= 0 ){
		return m_Worlds[idx];
	}

	naw	= NewObject<UNaWorld>();

	//! �G���g���₢���킹
	if ( FNaWorldRecord* rec = db->FindWorldEntry( id ) ){
		naw->OpenWorld( rec->DataID );
	}
	else {
		//! �V�K����
		naw->CreateWorld( id, assetID );
		db->RegisterWorldEntry( id, naw->GetDataID() );
	}

	naw->Setup( m_WorldActor );	//!@ ����WorldManager�ɕύX

	m_Worlds.Add( naw );

	return naw;
}

//! ���[���h�N���[�Y
void UNaWorldManager::CloseWorld( FName id )
{
	int32	idx;

	idx	= m_Worlds.IndexOfByPredicate( [id]( UNaWorld* p )
	{
		return p->GetUID() == id;
	});

	if ( idx >= 0 ){
		UNaWorld*	naw = m_Worlds[idx];

		naw->CloseWorld();

		if ( naw == m_RequestWorld ){
			m_RequestWorld	= nullptr;
		}
		if ( naw == m_ActiveWorld ){
			m_ActiveWorld	= nullptr;
		}
		m_Worlds.RemoveAt( idx );
	}
}

//! �A�N�e�B�u���[���h�ύX���N�G�X�g
void UNaWorldManager::ChangeWorld( FName id )
{
	int32	idx;

	idx	= m_Worlds.IndexOfByPredicate( [id]( const UNaWorld* p )
	{
		return p->GetUID() == id;
	});

	//! �J���Ă���ꍇ�̂ݓK�p
	if ( idx >= 0 ){
		m_RequestWorld	= m_Worlds[idx];
	}
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
