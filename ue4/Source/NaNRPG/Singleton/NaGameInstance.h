// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"

#include "Entity/Character/NaEntityPlayer.h"

#include "UI/Actor/NaUIAgent.h"

#include "NaGameInstance.generated.h"

class UNaGameDatabase;
class UNaAssetLibrary;
class UNaGameLogger;

/**
 * 
 */
UCLASS()
class NANRPG_API UNaGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
public:
	//! インスタンス取得
	static UNaGameInstance*	Get( UObject* WorldContext );

	//
	virtual void	Init() override;

	//! DB取得
	UNaGameDatabase*	GetDB() const		{return m_GameDB;}
	//! AssetLibrary取得
	UNaAssetLibrary*	GetALib() const		{return m_AssetLibrary;}
	//! ロガー取得
	UNaGameLogger*		GetLogger() const	{return m_GameLogger;}

	//! UI仲介クラス取得
	UClass*	FindUI( FName name )			{ return UIAgents.FindRef( name ); }

public:
	//! UI仲介クラス
	UPROPERTY(EditDefaultsOnly, Category = "UIAgent")
	TMap<FName, TSubclassOf<ANaUIAgent> >	UIAgents;
	
public:
	//
	UPROPERTY()
	UNaGameDatabase*	m_GameDB;
	//
	UPROPERTY()
	UNaAssetLibrary*	m_AssetLibrary;
	//!
	UPROPERTY()
	UNaGameLogger*		m_GameLogger;
};
