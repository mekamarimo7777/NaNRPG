// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaDatabaseConst.h"

#include "Entity/Character/NaEntityPlayer.h"

#include "NaGameDatabase.generated.h"

/**
 * グローバルゲームデータベース（最上位のセーブデータ）
 */
UCLASS()
class NANRPG_API UNaGameDatabase : public UObject
{
	GENERATED_BODY()
	
public:
	//
	static UNaGameDatabase*	GetDB();
	//
	static FString	GameSavedDir();

	//
	bool	LoadDB(FString name);
	//
	bool	SaveDB();

	//
	virtual	void	Serialize( FArchive& ar ) override;

	//! プレイヤーエンティティ登録
	void				RegisterPlayer( UNaEntityPlayer* player );
	//! グローバルエンティティ登録
	void				RegisterGlobalEntity( UNaEntity* entity );
	//! 指定ワールド内エンティティ収集
	void				GatherEntities( FName id, TArray<UNaEntity*>& outVal );
	//! プレイヤーエンティティ取得
	UNaEntityPlayer*	GetPlayer() const	{ return m_Player; }

	//! ワールドエントリ登録
	void				RegisterWorldEntry( FName id, uint32 dataID );
	//! ワールドエントリ取得
	FNaWorldRecord*		FindWorldEntry( FName id );
	//! ワールドエントリ確認
	bool				ExistWorldEntry( FName id );
	//! ワールドデータID生成
	uint32				GenerateWorldDataID();

protected:
	//
	FString	GetDBDirName();
	//
	FString	GetDBFileName();

public:
	FString	m_DataName;

	//! プレイヤー実体
	UPROPERTY()
	UNaEntityPlayer*			m_Player;
	//! パーティメンバー
	UPROPERTY()
	TArray<UNaEntityCharacter*>	m_PartyMember;
	//! グローバルエンティティ
	UPROPERTY()
	TArray<UNaEntity*>			m_GlobalEntities;

	//! 生成ワールド一覧
	UPROPERTY()
	TArray<FNaWorldRecord>	m_WorldRecords;
	//! カレントのローカルマップ（未設定時はグローバルマップ）
	UPROPERTY()
	FName					m_WorldID;

	//! イベントフラグ
//	UPROPERTY()
//	TBitArray<FDefaultBitArrayAllocator>	m_EventFlags;
	//! イベント変数
	UPROPERTY()
	TMap<FName, FString>					m_EventVariables;

protected:



};
