// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaEntitySpawnManager.h"

#include "Entity/INaEntityFactory.h"
#include "Entity/Character/NaEntityCharacter.h"

#include "Database/Item/NaItem.h"

#include "Assets/NaAssetLibrary.h"

#include "World/NaWorld.h"

//
void UNaEntitySpawnManager::OnAwake()
{
	m_TurnAction	= NewObject<UNaTurnActionComponent>();
	if ( m_TurnAction ){
		m_TurnAction->SetSpeed( 5 );

		m_TurnAction->BeginTurnEvent().BindUFunction( this, "OnBeginTurn" );
		m_TurnAction->UpdateTurnEvent().BindUFunction( this, "OnUpdateTurn" );
	}

	//@obsolete
	m_bAbstract	= true;
	m_Speed		= 5;
	m_SpawnMax	= 10;
}

// ターン開始
void UNaEntitySpawnManager::OnBeginTurn()
{
}

// ターン更新
void UNaEntitySpawnManager::OnUpdateTurn( float DeltaTime )
{
/*	if ( --m_SpawnMax < 0 ){
		m_TurnAction->EndTurn();
		return;
	}*/

	UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
	UNaWorld*			naw = GetNaWorld();

	const TArray<UNaEntity*>&	entities = naw->GetEntities();

	if ( entities.Num() >= m_SpawnMax ){
		m_TurnAction->EndTurn();
		return;
	}

	if ( alib ){
		const FNaEntityDataAsset*	asset = nullptr;

		if ( FMath::RandRange( 0, 3 ) == 0 ){
//		if ( false ){
			asset	= alib->FindEntityAsset( "Jelline" );

			if ( asset ){
				UNaEntityCharacter*	chara;
				
				chara	= INaEntityFactory::NewEntity<UNaEntityCharacter>( asset->Type );
				if ( chara ){
					FIntVector	pos;

					chara->CreateFromAsset( *asset );

					pos.X	= 0;
					pos.Y	= 0;
					pos.Z	= 10;
					naw->SpawnEntity( chara, pos );

	//				GEngine->AddOnScreenDebugMessage( -1, 1.0f, FColor::Yellow, "SpawnEntity!" );
				}
			}
		}
		else {
			UNaItem*	item;

			item	= GenerateItem( 0, 0xFFFFFFFF );
			if ( item ){
				UNaEntityCharacter*	chara;

				chara	= item->GenerateEntity();
				if ( chara ){
					FIntVector	pos, tpos;

					pos.X	= FMath::RandRange( -10, 10 );
					pos.Y	= FMath::RandRange( -10, 10 );
					pos.Z	= 10;
					naw->FindGroundPos( pos, tpos );

					naw->SpawnEntity( chara, tpos );
				}
			}
		}
	}

	m_TurnAction->EndTurn();
}

//!
UNaItem* UNaEntitySpawnManager::GenerateItem( int32 intensity, int32 typeMask )
{
	UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
	UNaItem*			item = nullptr;

	{
		const TMap<FName, FNaItemDataAsset*>&	items = alib->GetItemList();
		TArray<FNaItemDataAsset*>	candidates;
		FNaItemDataAsset*			asset = nullptr;

		for ( auto it : items ){
			candidates.Add( it.Value );
		}

		if ( candidates.Num() ){
			asset	= candidates[ FMath::RandRange( 0, candidates.Num() - 1 ) ];
		}

		if ( asset ){
			item	= NewObject<UNaItem>();
			item->CreateFromAsset( *asset );
			item->AddStack( FMath::RandRange( 1, asset->MaxStackCount ) );
		}
	}

	return item;
}
