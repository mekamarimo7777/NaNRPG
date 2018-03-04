// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaUICategoryList.h"


//!
void UNaUICategoryList::NativeConstruct()
{
	Super::NativeConstruct();

	if ( !m_LvMain ){
		m_LvMain	= Cast<UNaListView>( WidgetTree->FindWidget( "MainList" ) );
		if ( m_LvMain ){
			m_LvMain->SelectedIndexChanged().AddLambda( [this]( int32 index )
			{
				m_CategoryChangedEvent.Broadcast( index );
			});
		}
	}
}

//!
void UNaUICategoryList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );
}

//!
void UNaUICategoryList::AddItem( FString name, FString text, FString iconPath )
{
	FNaListItem	lvi;

	lvi.Value	= text;
	lvi.Params.Add( "category", name );
	lvi.Params.Add( "icon", iconPath );
	m_LvMain->AddItem( lvi );
}

//!
void UNaUICategoryList::DeleteAllItems()
{
	m_LvMain->DeleteAllItems();
}

//! 
FName UNaUICategoryList::GetCategoryName( int32 idx ) const
{
	FString	str;

	str	= m_LvMain->GetItemParameter( idx, "category" );

	return FName( *str );
}
