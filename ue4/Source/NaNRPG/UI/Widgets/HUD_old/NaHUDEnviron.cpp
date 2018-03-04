// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaHUDEnviron.h"

#include "Database/Item/NaItem.h"
#include "Entity/Character/NaEntityCharacter.h"

//!
void UNaHUDEnviron::NativeConstruct()
{
	Super::NativeConstruct();

}

//!
void UNaHUDEnviron::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );
}

//!
FReply UNaHUDEnviron::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{

	return Super::NativeOnFocusReceived( InGeometry, InFocusEvent );
}
