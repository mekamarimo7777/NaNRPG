// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaGameDatabase.h"

#include "Singleton/NaGameInstance.h"


//
UNaGameDatabase* UNaGameDatabase::GetDB()
{
	UNaGameInstance*	gi = Cast<UNaGameInstance>( UGameplayStatics::GetGameInstance( GWorld ) );

	if ( gi ){
		return gi->GetDB();
	}
	else {
		return nullptr;
	}
}

//
FString UNaGameDatabase::GameSavedDir()
{
	UNaGameDatabase*	db = GetDB();

	if ( db ){
		return db->GetDBDirName();
	}
	else {
		return FPaths::ProjectSavedDir();
	}
}

//
bool UNaGameDatabase::LoadDB(FString name)
{
	IPlatformFile&	pf = FPlatformFileManager::Get().GetPlatformFile();
	FString			dir,fname;

	m_DataName	= name;

	dir	= GetDBDirName();
	if ( !FPaths::DirectoryExists( dir ) ){
		pf.CreateDirectoryTree( *dir );
	}

	fname	= GetDBFileName();
	if ( FPaths::DirectoryExists( fname ) ){
		return false;
	}

	if ( FPaths::FileExists( fname ) ){
		TArray<uint8>	buff;

		if ( FFileHelper::LoadFileToArray( buff, *fname ) ){
			FMemoryReader	reader( buff );
			Serialize( reader );
		}
	}
	else {
		// 新規作成 //
		SaveDB();
	}

	return true;
}

//
bool UNaGameDatabase::SaveDB()
{
	IPlatformFile&	pf = FPlatformFileManager::Get().GetPlatformFile();
	FString			fname;

	fname	= GetDBFileName();
	if ( FPaths::DirectoryExists( fname ) ){
		return false;
	}

	FBufferArchive	ar;

	Serialize( ar );
	FFileHelper::SaveArrayToFile( ar, *fname );

	return true;
}

//
void UNaGameDatabase::Serialize( FArchive& ar )
{
	if ( ar.IsSaving() ){

	}
	else if ( ar.IsLoading() ){
	}
}

//! プレイヤーエンティティ登録
void UNaGameDatabase::RegisterPlayer( UNaEntityPlayer* player )
{
	m_Player	= player;
	m_Player->SetStage( ENaEntityStage::Global );
}

//! グローバルエンティティ登録
void UNaGameDatabase::RegisterGlobalEntity( UNaEntity* entity )
{
	entity->SetStage( ENaEntityStage::Global );
	m_GlobalEntities.Add( entity );
}

//! 指定ワールド内エンティティ収集
void UNaGameDatabase::GatherEntities( FName id, TArray<UNaEntity*>& outVal )
{
	outVal	= m_GlobalEntities.FilterByPredicate( [id]( UNaEntity* p )
	{
		return p->GetWorldID() == id;
	});

	if ( m_Player->GetWorldID() == id ){
		outVal.Add( m_Player );
	}
}

//! ワールドエントリ登録
void UNaGameDatabase::RegisterWorldEntry( FName id, uint32 dataID )
{
	FNaWorldRecord	rec;

	rec.UID		= id;
	rec.DataID	= dataID;

	m_WorldRecords.Add( rec );
}

//! ワールドエントリ取得
FNaWorldRecord* UNaGameDatabase::FindWorldEntry( FName id )
{
	FNaWorldRecord*	rec;

	rec	= m_WorldRecords.FindByPredicate( [id]( FNaWorldRecord& p )
	{
		return p.UID == id;
	});

	return rec;
}

//! ワールドエントリ確認
bool UNaGameDatabase::ExistWorldEntry( FName id )
{
	return m_WorldRecords.FindByPredicate( [id]( FNaWorldRecord& p )
	{
		return p.UID == id;
	}) != nullptr;
}

//! ワールドデータID生成
uint32 UNaGameDatabase::GenerateWorldDataID()
{
	uint32	dataID;

#if 0
	//! ランダムでID生成
	do {
		dataID	= FMath::Rand();
	} while ( m_WorldRecords.ContainsByPredicate( [dataID]( FNaWorldRecord& p ){ return p.DataID == dataID; }) );
#else
	dataID	= m_WorldRecords.Num();
#endif

	return dataID;
}

//
FString	UNaGameDatabase::GetDBDirName()
{
	return FPaths::Combine( *FPaths::ProjectSavedDir(), *FString::Printf( TEXT("GameSaves/%s"), *m_DataName ) );
}
//
FString	UNaGameDatabase::GetDBFileName()
{
	return FPaths::Combine( *GetDBDirName(), TEXT("db.nadb") );
}
