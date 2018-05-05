// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "INaEntityFactory.h"

#include "Entity/Character/NaEntityPlayer.h"

#include "Entity/Env/NaEntitySpawnManager.h"

// デフォルトのやつ
class UNaEntityFactoryImpl : public INaEntityFactory
{
protected:
	//!
	virtual UNaEntity* NewEntity_Inner( ENaEntity::Type type ) override
	{
		UNaEntity*	entity = nullptr;

		switch ( type ){
		case ENaEntity::Character:
		case ENaEntity::Item:
			entity	= NewObject<UNaEntityCharacter>();
			break;
		case ENaEntity::Player:
			entity	= NewObject<UNaEntityPlayer>();
			break;
		case ENaEntity::SpawnManager:
			entity	= NewObject<UNaEntitySpawnManager>();
			break;
		}

		if ( entity ){
			entity->Initialize( type );
		}

		return entity;
	}
};

//!
TSharedPtr<INaEntityFactory>	INaEntityFactory::s_EntityFactory;

//!
INaEntityFactory* INaEntityFactory::GetFactory()
{
	if ( !s_EntityFactory.IsValid() ){
		s_EntityFactory	= TSharedPtr<INaEntityFactory>( new UNaEntityFactoryImpl );
	}
	return s_EntityFactory.Get();
}

//! エンティティシリアライズ
void INaEntityFactory::SerializeEntities( FArchive& ar, TArray<UNaEntity*>& refArray )
{
	TEnumAsByte<ENaEntity::Type>	etype;
	int32	tmpNum;

	if ( ar.IsSaving() ){
		tmpNum	= refArray.Num();
		ar << tmpNum;

		for ( auto& it : refArray ){
			etype	= it->GetType();
			ar << etype;
			it->Serialize( ar );
		}
	}
	else if ( ar.IsLoading() ){
		UNaEntity*	entity;

		ar << tmpNum;
		refArray.SetNum( tmpNum );

		for ( int32 i = 0; i < tmpNum; ++i ){
			ar << etype;
			entity	= INaEntityFactory::NewEntity( etype );
			if ( entity ){
				entity->Serialize( ar );
				refArray[i]	= entity;
			}
		}
	}
}

// ====================================================================================================

//
INaEntityFactory::INaEntityFactory()
{
}
