// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaUIPartyList.h"

#include "Entity/Character/NaEntityCharacter.h"

//!
void UNaUIPartyList::NativeConstruct()
{
	Super::NativeConstruct();

	if ( !m_LvMain ){
		m_LvMain	= Cast<UNaListView>( WidgetTree->FindWidget( "MainList" ) );
	}
}

//!
void UNaUIPartyList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );
}

//!
FReply UNaUIPartyList::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if ( m_LvMain ){
		m_LvMain->SetKeyboardFocus();
		return FReply::Handled();
	}

	return Super::NativeOnFocusReceived( InGeometry, InFocusEvent );
}

//!
void UNaUIPartyList::DeleteAllItems()
{
	m_LvMain->DeleteAllItems();
}

//!
void UNaUIPartyList::AddMember( UNaEntityCharacter* chara )
{
	FNaEntityProfile*	profile;
	FNaCharacterStatus*	status;
	FNaListItem	lvi;

	profile	= chara->GetProfile();
	status	= chara->GetStatus();

	lvi.Value	= profile->DisplayName;
	lvi.Params.Add( "hp", FString::FromInt( status->HP ) );
	lvi.Params.Add( "mhp", FString::FromInt( status->HPMax ) );
	m_LvMain->AddItem( lvi );
}
