// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	//! プレイヤーエンティティ取得
	UNaEntityPlayer*	GetPlayer() const	{ return m_Player; }

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
	//! その他汎用グローバルエンティティ
	UPROPERTY()
	TArray<UNaEntity*>			m_GlobalEntities;

	//! カレントのローカルマップ
	UPROPERTY()
	FName	m_CurrentWorld;
	//! 非ユニークマップの払い出しID
	UPROPERTY()
	int32	m_NextWorldID;

	//! イベントフラグ
//	UPROPERTY()
//	TBitArray<FDefaultBitArrayAllocator>	m_EventFlags;
	//! イベント変数
	UPROPERTY()
	TMap<FName, FString>					m_EventVariables;

protected:



};
