// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaUIItemList.h"

#include "Database/Item/NaItem.h"
#include "Entity/Character/NaEntityCharacter.h"

//!
void UNaUIItemList::NativeConstruct()
{
	Super::NativeConstruct();

	if ( !m_LvMain ){
		m_LvMain	= Cast<UNaListView>( WidgetTree->FindWidget( "MainList" ) );
	}
}

//!
void UNaUIItemList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );
}

//!
FReply UNaUIItemList::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if ( m_LvMain ){
		m_LvMain->SetKeyboardFocus();
		return FReply::Handled();
	}

	return Super::NativeOnFocusReceived( InGeometry, InFocusEvent );
}

//!
void UNaUIItemList::DeleteAllItems()
{
	m_LvMain->DeleteAllItems();
}

//!
void UNaUIItemList::AddText( FString text )
{
	FNaListItem	lvi;

	lvi.Value	= text;
	lvi.Params.Add( "icon", FString() );
	lvi.Params.Add( "hsv", FString() );
	lvi.Params.Add( "stack", FString() );
//	lvi.Params.Add( "max_stack", FString::FromInt( item->GetMaxStackCount() ) );
	m_LvMain->AddItem( lvi );
}

//!
void UNaUIItemList::AddItem( UNaItem* item )
{
	FNaListItem	lvi;

	lvi.Value	= item->GetDisplayName();
	lvi.Params.Add( "icon", FString::FromInt( item->GetIconID() ) );
	lvi.Params.Add( "hsv", item->GetHSV().ToString() );
	lvi.Params.Add( "stack", FString::FromInt( item->GetStackCount() ) );
//	lvi.Params.Add( "max_stack", FString::FromInt( item->GetMaxStackCount() ) );
	m_LvMain->AddItem( lvi );
}

//! 
void UNaUIItemList::DeleteItem( int32 index )
{
	m_LvMain->DeleteItem( index );
}
