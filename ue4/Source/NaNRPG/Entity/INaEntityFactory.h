// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Misc/NaEnums.h"

class UNaEntity;

/**
 * エンティティファクトリ
 */
class NANRPG_API INaEntityFactory
{
public:
	// 
	static INaEntityFactory* GetFactory();

	// エンティティインスタンス生成
	template<typename T = UNaEntity>
	static T*	NewEntity( ENaEntity::Type type )
	{
		return static_cast<T*>( GetFactory()->NewEntity_Inner( type ) );
	}

	//! エンティティシリアライズ
	static void	SerializeEntities( FArchive& ar, TArray<UNaEntity*>& refArray );

public:
	//
	INaEntityFactory();

protected:
	//! 
	virtual UNaEntity*	NewEntity_Inner( ENaEntity::Type type ) = 0;

public:
	static TSharedPtr<INaEntityFactory>	s_EntityFactory;
};
