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
//! コンストラクタ
UNaWorldManager::UNaWorldManager()
: m_CurrentAction( nullptr )
, m_EventManager( nullptr )
, m_WorldActor( nullptr )
, m_WorldContext( nullptr )
{
}

//! 初期化
void UNaWorldManager::Initialize( ANaWorldActor* actor )
{
	m_WorldActor	= actor;
	m_WorldContext	= m_WorldActor->GetWorld();

	m_EventManager	= NewObject<UNaEventManager>();
	m_EventManager->Initialize( this );
}

//! 更新処理
void UNaWorldManager::Update( float DeltaTime )
{
	UNaEntity*	focusEntity = GetFocusEntity();

	//! クローズ済みワールドの削除
	m_Worlds.RemoveAll( []( const UNaWorld* p )
	{
		return p->IsClosed();
	});

	//! 各ワールド更新
	for ( auto& it : m_Worlds ){
		if ( focusEntity->GetWorldID() == it->GetUID() ){
			it->SetViewOrigin( focusEntity->GetWorldPosition() );
		}
		else {
			it->SetViewOrigin( FIntVector::ZeroValue );
		}
		it->Update( DeltaTime );
	}

	//! アクティブワールド変更
	if ( m_RequestWorld != nullptr ){
		m_ActiveWorld	= m_RequestWorld;
		m_RequestWorld	= nullptr;
	}

	//! ターンアクション
	while ( true ){
		// 削除待ちエンティティ削除
		for ( auto& it : m_Worlds ){
			it->SweepEntities();
		}

		// ターンアクション開始
		if ( !m_CurrentAction && m_ActionChain.Num() > 0 ){
			m_CurrentAction	= m_ActionChain[0];
			m_ActionChain.RemoveAt( 0 );

			for ( auto it : m_ActionChain ){
				it->DecreaseWaitTime( m_CurrentAction->GetWaitTime() );
			}
			m_CurrentAction->BeginTurn();
		}

		// ターン処理
		if ( m_CurrentAction ){
			m_CurrentAction->ExecuteTurn( 0.0f );

			// 終了しなかった場合は次フレームへ
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

	//! ゲームイベント処理
	if ( m_EventManager ){
		m_EventManager->Tick( DeltaTime );
	}
}

//! アクションチェインに追加
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
void UNaWorldManager::InsertActionChain( UNaEntity* entity )
{
	if ( entity->HasTurnAction() ){
		InsertActionChain( entity->GetTurnAction() );
	}
}

//! アクションチェインから除去
void UNaWorldManager::RemoveActionChain( UNaTurnActionComponent* tac )
{
	m_ActionChain.Remove( tac );

	if ( m_CurrentAction == tac ){
		m_CurrentAction	= nullptr;
	}
}
void UNaWorldManager::RemoveActionChain( UNaEntity* entity )
{
	if ( entity->HasTurnAction() ){
		RemoveActionChain( entity->GetTurnAction() );
	}
}

//! ワールドオープン
UNaWorld* UNaWorldManager::OpenWorld( FName id, FName assetID )
{
	UNaGameDatabase*	db = UNaGameDatabase::GetDB();
	UNaWorld*	naw;
	int32		idx;

	idx	= m_Worlds.IndexOfByPredicate( [id]( UNaWorld* p )
	{
		return p->GetUID() == id;
	});

	//! すでに開いている場合
	if ( idx >= 0 ){
		return m_Worlds[idx];
	}

	naw	= NewObject<UNaWorld>();

	//! エントリ問い合わせ
	if ( FNaWorldRecord* rec = db->FindWorldEntry( id ) ){
		naw->OpenWorld( rec->DataID );
	}
	else {
		//! 新規生成
		naw->CreateWorld( id, assetID );
		db->RegisterWorldEntry( id, naw->GetDataID() );
	}

	naw->Setup( m_WorldActor );	//!@ 今後WorldManagerに変更

	m_Worlds.Add( naw );

	return naw;
}

//! ワールドクローズ
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
	}
}

//! アクティブワールド変更リクエスト
void UNaWorldManager::ChangeWorld( FName id )
{
	int32	idx;

	idx	= m_Worlds.IndexOfByPredicate( [id]( const UNaWorld* p )
	{
		return p->GetUID() == id;
	});

	//! 開いている場合のみ適用
	if ( idx >= 0 ){
		m_RequestWorld	= m_Worlds[idx];
	}
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//! 表示ターゲット取得
UNaEntity* UNaWorldManager::GetFocusEntity() const
{
	UNaGameDatabase*	db = UNaGameDatabase::GetDB();
	UNaEntity*			player = db->GetPlayer();

	return player;
}
