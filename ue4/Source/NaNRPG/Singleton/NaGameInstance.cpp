// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaGameInstance.h"

#include "Assets/NaAssetLibrary.h"

#include "Database/NaGameDatabase.h"

#include "NaGameLogger.h"

//! インスタンス取得
UNaGameInstance* UNaGameInstance::Get( UObject* WorldContext )
{
	return Cast<UNaGameInstance>( UGameplayStatics::GetGameInstance( WorldContext ) );
}

//! 
void UNaGameInstance::Init()
{
	//! アセットライブラリ構築
#if 1
	UNaAssetLibrary*	lib = UNaAssetLibrary::Get();
	if ( lib ){
		lib->Initialize();
	}
#else
	m_AssetLibrary	= NewObject<UNaAssetLibrary>();
	if ( m_AssetLibrary ){
		m_AssetLibrary->Initialize();
	}
#endif

	//! データベース構築
	m_GameDB		= NewObject<UNaGameDatabase>();

	//! ロガー作成
	m_GameLogger	= NewObject<UNaGameLogger>();
	if ( m_GameLogger ){
		m_GameLogger->Initialize();
	}
}
