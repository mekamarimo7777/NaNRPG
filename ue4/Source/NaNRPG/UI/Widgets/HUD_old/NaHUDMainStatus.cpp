// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaHUDMainStatus.h"

#include "Entity/Character/NaEntityCharacter.h"

//!
void UNaHUDMainStatus::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );
}

//! 
void UNaHUDMainStatus::BindTarget( UNaEntityCharacter* chara )
{
	m_Target	= chara;
}

//! 
FText UNaHUDMainStatus::GetTargetName() const
{
	if ( m_Target ){
		return FText::FromString( m_Target->GetDisplayName() );
	}
	else {
		return FText();
	}
}

//! 
int32 UNaHUDMainStatus::GetHealth() const
{
	if ( m_Target ){
		FNaCharacterStatus*	status = m_Target->GetStatus();

		return status->HP;
	}
	else {
		return 9999;
	}
}
//! 
int32 UNaHUDMainStatus::GetMaxHealth() const
{
	if ( m_Target ){
		FNaCharacterStatus*	status = m_Target->GetStatus();

		return status->HPMax;
	}
	else {
		return 9999;
	}
}
