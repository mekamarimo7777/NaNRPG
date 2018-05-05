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
 * エンティティ基底
 */
UCLASS(abstract)
class NANRPG_API UNaEntity : public UObject
{
	GENERATED_BODY()

public:
	// コンストラクタ
	UNaEntity();

	// 初期化
	void	Initialize( ENaEntity::Type type );

	// ID設定
	void	SetID( int32 id );
	// ID除去
	void	ResetID();

	// スポーン処理（チャンクデータへの追加）
	void	Spawn();
	// デスポーン処理（チャンクデータから削除）
	void	Despawn();
	// 入場処理（チャンク起動時の処理）
	void	Enter();
	// 退場処理（チャンク消去時の退避処理）
	void	Leave();

	// Naワールド設定
	void		SetNaWorld( UNaWorld* world )	{ m_pWorld = world; }
	// Naワールド取得
	UNaWorld*	GetNaWorld() const				{ return m_pWorld; }

	//! 所属ワールドID設定
	void		SetWorldID( FName id )			{ m_WorldID = id; }
	//! 所属ワールドID取得
	FName		GetWorldID() const				{ return m_WorldID; }
	//! 所属マップID設定
	void		SetMapID( int32 id )			{ m_MapID = id; }
	//! 所属マップID取得
	int32		GetMapID() const				{ return m_MapID; }

	// UEワールド取得
	UWorld*		GetWorldContext() const			{ return m_pWorld ? m_pWorld->GetWorldContext() : nullptr; }
	// UEHUD取得
	ANaGameHUD*	GetHUD() const					{ return Cast<ANaGameHUD>( GWorld->GetFirstPlayerController()->GetHUD() ); }

	//! エンティティ情報生成
	virtual void	CreateFromAsset( const FNaEntityDataAsset& asset );
	//! エンティティパラメータ設定
	virtual void	SetEntityParameter( const TMap<FName, FString>& values );

	//!
	FNaEntityProfile*	GetProfile()		{return &m_Profile;}
	//!
	FString				GetDisplayName()	{return m_Profile.DisplayName;}

	//! アイテム情報セット
	virtual void		SetItemProperty( UNaItem* item )	{}
	//! アイテム情報取得
	virtual UNaItem*	GetItemProperty()					{ return nullptr; }

	//! スポーン判定
	bool	IsSpawned() const	{ return m_Spawned; }
	//! 生存判定
	bool	IsAlive() const		{ return !m_IsKill; }

	//! イベントセット
	void	SetEvent( FName eventID );
	//! イベント取得
	bool	FindEvent( ENaEventTrigger trigger, const UNaEventAsset*& outEvent, int32& outSheet );






	// 座標設定
	virtual void	SetWorldPosition(const FIntVector& pos);
	virtual void	MoveToWorldPosition(const FIntVector& pos);
	void			UpdateCurrentChunk();
	//! ワールド座標取得
	FIntVector		GetWorldPosition() const				{ return m_WorldPos; }
	//! 所属チャンク座標取得
	FIntVector		GetChunkPosition() const;

	// 向き設定
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
	// ステート
	//==================================================
	//!
	virtual UNaStateMachine*	GetStateMachine() const			{ return m_SM; }

	//==================================================
	// ターンアクション
	//==================================================
	//!
	virtual UNaTurnActionComponent*	GetTurnAction() const	{ return nullptr; }
	//!
	FORCEINLINE bool	HasTurnAction() const	{ return GetTurnAction() != nullptr; }

	// 削除
	void	Kill()	{ m_IsKill = true; }
	// 削除待ち
	virtual bool	IsPendingKill() const	{ return m_IsKill; }

	//!
	virtual UNaEntityManipulator*	GetManipulator() const			{ return nullptr; }

	//
	void					SetStage( ENaEntityStage::Type stage )	{m_Stage = stage;}
	//
	ENaEntityStage::Type	GetStage() const	{return m_Stage;}

	//! 無形エンティティ（チャンクに所属しない）
	virtual bool	IsIntangible() const	{ return m_IsAbstract; }
	// コリジョンの有無
	virtual bool	IsCollidable() const	{ return !m_IsAbstract && m_Collidable; }

	// 
	//virtual bool	IsAbstract() const		{return m_IsAbstract;}
	// 
	//virtual bool	IsStationaly() const	{return m_IsStationary;}
	// 
	//virtual bool	IsNoSave() const		{return m_bNoSave;}

	// シリアライズ
	virtual	void	Serialize( FArchive& ar ) override;

protected:
	// 生成後の初期化
	virtual void	OnAwake(){}
	// 削除前
	virtual void	OnDestroy(){}
	// スポーン後処理
	virtual void	OnSpawn(){}
	// デスポーン前処理
	virtual void	OnDespawn(){}
	// 
	virtual void	OnEnter(){}
	// 
	virtual void	OnLeave(){}

	//! シリアライズ処理
	virtual void	OnSerialize( FArchive& ar );

	//! 一時情報の更新
	void	UpdateTransientData( const FNaEntityDataAsset* asset );

public:
	//! アクションしない（アクションチェインに繋がない）
	bool	m_IsStationary;
	//! 座標管理しないエンティティ（チャンクに属さない）
	bool	m_IsAbstract;
	//! キルフラグ
	bool	m_IsKill;
	
protected:
	//*** Serialize ***//
	//! エンティティ種別
	TEnumAsByte<ENaEntity::Type>	m_Type;
	//! アセットID
	FName							m_AssetID;
	//! 固有ID
	uint32							m_ID;
	//! エンティティ情報
	FNaEntityProfile				m_Profile;

	//! 所属レベル
	TEnumAsByte<ENaEntityStage::Type>	m_Stage;
	//! 所属ワールドID（Worldステージ以上に設定）
	FName								m_WorldID;
	//! 所属マップID（-1で未所属）
	int32								m_MapID;
	//! 
	bool								m_Spawned;

	FIntVector		m_WorldPos;		// ワールド座標 //
	ENaDirection	m_Direction;	// 向き //

	//
	int32	m_Speed;			// 行動速度 //
	int32	m_WaitTime;			// 待ち時間 //
	uint32	m_TotalTurn;		// 累計ターン //

	int32	m_Group;		// 所属勢力（仮） //

	//
	int32	m_LastUpdateTime;	// 前回の行動時間 //
	int32	m_RevivalTime;		// 復活待ち時間（復活するキャラのみ） //

	//! イベント
	FName					m_EventID;
	//! エンティティパラメータ
	TMap<FName, FString>	m_Params;

	//*** Serialize End ***//

protected:
	// 一時パラメータ //
	//! 所属ワールド参照
	UPROPERTY(Transient)
	UNaWorld*	m_pWorld;
	//! 所属チャンク参照
	UPROPERTY(Transient)
	UNaChunk*	m_pCurrentChunk;

	//! ステート管理
	UPROPERTY(Transient)
	UNaStateMachine*	m_SM;

	//! ソースアセット
	const FNaEntityDataAsset*	m_Asset;
	//! サイズ
	FIntVector					m_Size;
	//! コリジョン有無
	bool						m_Collidable;
};
