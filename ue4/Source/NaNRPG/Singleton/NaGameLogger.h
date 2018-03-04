// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaGameLogger.generated.h"

class UNaEntity;
class UNaItem;
class UNaLogWidget;
class UNaLogDefinition;

struct FNaLogMessage;

/**
 * 
 */
UCLASS()
class NANRPG_API UNaGameLogger : public UObject
{
	GENERATED_BODY()
	
public:
	//!
	static UNaGameLogger*	Get();

	//!
	void	Initialize();

	//!
	void	LogDamage( UNaEntity* entity, UNaEntity* target, int32 value );
	//!
	void	LogDestroy( UNaEntity* entity );
	//!
	void	LogTakeItem( UNaEntity* entity, UNaItem* item );
	//!
	void	LogAction( UNaEntity* entity, UNaItem* item, ENaActionTrigger::Type action );
	//!
	void	LogEquip( UNaEntity* entity, UNaItem* item, bool isEquip );

protected:
	//! 
	void	DisplayLogText( const FString& str, FLinearColor color );
	//! 
	const FNaLogMessage*	GetLogMessage( FName name ) const;

public:

protected:
	//! 
	UPROPERTY()
	UNaLogDefinition*	m_LogDefinition;



};
