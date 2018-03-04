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
		// �V�K�쐬 //
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

//! �v���C���[�G���e�B�e�B�o�^
void UNaGameDatabase::RegisterPlayer( UNaEntityPlayer* player )
{
	m_Player	= player;
	m_Player->SetStage( ENaEntityStage::Global );
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
