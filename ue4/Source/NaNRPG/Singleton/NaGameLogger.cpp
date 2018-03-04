// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaGameLogger.h"

#include "Singleton/NaGameInstance.h"

#include "Entity/NaEntity.h"
#include "Database/Item/NaItem.h"

#include "HUD/NaGameHUD.h"

//
UNaGameLogger* UNaGameLogger::Get()
{
	UNaGameInstance*	gi = Cast<UNaGameInstance>( UGameplayStatics::GetGameInstance( GWorld ) );

	if ( gi ){
		return gi->GetLogger();
	}
	else {
		return nullptr;
	}
}

//!
void UNaGameLogger::Initialize()
{
	m_LogDefinition	= LoadObject<UNaLogDefinition>( this, TEXT("/Game/Assets/Text/DA_LogDefinition.DA_LogDefinition") );
}

//!
void UNaGameLogger::LogDamage( UNaEntity* entity, UNaEntity* target, int32 value )
{
	const FNaLogMessage*	log = GetLogMessage( "Damage" );

	if ( log ){
		FString	str;

		str	= log->Text.ToString();
		str	= str.Replace( TEXT("{actor}"), *entity->GetDisplayName() );
		str	= str.Replace( TEXT("{target}"), *target->GetDisplayName() );
		str	= str.Replace( TEXT("{value}"), *FString::FromInt(value) );

		DisplayLogText( str, log->Color );
	}
}

//!
void UNaGameLogger::LogDestroy( UNaEntity* entity )
{
	const FNaLogMessage*	log = GetLogMessage( "Destroy" );

	if ( log ){
		FString	str;

		str	= log->Text.ToString();
		str	= str.Replace( TEXT("{actor}"), *entity->GetDisplayName() );

		DisplayLogText( str, log->Color );
	}
}

//!
void UNaGameLogger::LogTakeItem( UNaEntity* entity, UNaItem* item )
{
	const FNaLogMessage*	log = GetLogMessage( "TakeItem" );

	if ( log ){
		FString	str;

		str	= log->Text.ToString();
		str	= str.Replace( TEXT("{actor}"), *entity->GetDisplayName() );
		str	= str.Replace( TEXT("{item}"), *item->GetDisplayName() );
		str	= str.Replace( TEXT("{value}"), *FString::FromInt( item->GetStackCount() ) );

		DisplayLogText( str, log->Color );
	}
}

//!
void UNaGameLogger::LogAction( UNaEntity* entity, UNaItem* item, ENaActionTrigger::Type action )
{
	const FNaLogMessage*	log = GetLogMessage( "Action" );

	if ( log ){
		FString	str;

		str	= log->Text.ToString();
		str	= str.Replace( TEXT("{actor}"), *entity->GetDisplayName() );
		str	= str.Replace( TEXT("{item}"), *item->GetDisplayName() );
		str	= str.Replace( TEXT("{action}"), *m_LogDefinition->GetActionText( action ).ToString() );

		DisplayLogText( str, log->Color );
	}
}

//!
void UNaGameLogger::LogEquip( UNaEntity* entity, UNaItem* item, bool isEquip )
{
	const FNaLogMessage*	log = GetLogMessage( isEquip ? "Equip" : "EquipRemove" );
	
	if ( log ){
		FString	str;

		str	= log->Text.ToString();
		str	= str.Replace( TEXT("{actor}"), *entity->GetDisplayName() );
		str	= str.Replace( TEXT("{item}"), *item->GetDisplayName() );

		DisplayLogText( str, log->Color );
	}
}

//! 
void UNaGameLogger::DisplayLogText( const FString& str, FLinearColor color )
{
/*	APlayerController*	pc = UGameplayStatics::GetPlayerController( GWorld, 0 );
	ANaGameHUD*			hud;

	hud	= Cast<ANaGameHUD>( pc->GetHUD() );
	if ( hud ){
		UNaMainHUD*		uihud = hud->GetMainHUD();
		UNaLogWidget*	widget;

		widget	= uihud->GetLogWidget();
		if ( widget ){
			widget->AddText( str, color );
		}
	}*/
}

//! 
const FNaLogMessage* UNaGameLogger::GetLogMessage( FName name ) const
{
	if ( m_LogDefinition ){
		return m_LogDefinition->FindMessage( name );
	}
	else {
		return nullptr;
	}
}
