// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaListViewItem.h"

#include "UI/Controls/NaListView.h"

//!
void UNaListViewItem::NativeConstruct()
{
	Super::NativeConstruct();

	OnItemSelected( false );
}

//!
FReply UNaListViewItem::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	FReply	rep = Super::NativeOnFocusReceived( InGeometry, InFocusEvent );

	m_FocusedEvent.Broadcast();

	return rep;
}

//!
FReply UNaListViewItem::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply	rep = Super::NativeOnKeyDown( InGeometry, InKeyEvent );

	return rep;
}

//! 初期化
void UNaListViewItem::Setup( UNaListView* owner )
{
	m_Owner	= owner;
	if ( m_Owner ){
		bIsFocusable	= m_Owner->IsFocusable;
	}
}

//! アイテムデータ設定
void UNaListViewItem::SetItem( const FNaListItem& item )
{
	ItemValue	= item;
	m_Parameters.Append( item.Params );
}

//! アイテムデータ取得
FNaListItem* UNaListViewItem::GetItem()
{
	return &ItemValue;
}

//!
void UNaListViewItem::SelectItem( bool select )
{
	if ( m_IsSelected != select ){
		m_IsSelected	= select;

		if ( bIsFocusable ){
			if ( m_IsSelected && !HasKeyboardFocus() ){
				SetKeyboardFocus();
			}
		}

		OnItemSelected( m_IsSelected );
	}
}

//!
void UNaListViewItem::SetValue( FString value )
{
	ItemValue.Value	= value;
}

//! 
FString UNaListViewItem::GetValue() const
{
	return ItemValue.Value;
}
