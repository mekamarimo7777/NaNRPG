// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaListView.h"

//! 内部Box
class SNaBox : public SBox
{
	friend class UNaListView;

public:
	//! 
	virtual bool SupportsKeyboardFocus() const override
	{
		return m_Owner ? m_Owner->IsFocusable : false;
	}

	//!
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override
	{
		return m_Owner ? m_Owner->NativeOnFocusReceived( MyGeometry, InFocusEvent ) : FReply::Unhandled();
	}

	//! 
	virtual void OnFocusLost( const FFocusEvent& InFocusEvent ) override
	{
	}

	//! 
	virtual FReply OnKeyDown( const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent ) override
	{
		return m_Owner->NativeOnKeyDown( MyGeometry, InKeyEvent );
	}

protected:
	//! オーナーウィジェット
	UNaListView*	m_Owner;
};

//!
UNaListView::UNaListView( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer )
, m_CurSel( -1 )
, IgnoreKeyEvent( false )
{
}

//!
FReply UNaListView::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if ( m_CurSel >= 0 ){
		m_ListItems[m_CurSel]->SetKeyboardFocus();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

//!
FReply UNaListView::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if ( !InKeyEvent.IsRepeat() && !IgnoreKeyEvent ){
		const FKey	key = InKeyEvent.GetKey(); 

		if ( key == EKeys::Enter ){
			return FReply::Handled();
		}
		else if ( key == EKeys::Escape ){
			return FReply::Handled();
		}
	}

	return FReply::Unhandled();
}

//!
void UNaListView::AddItem( const FNaListItem& lvi )
{
	UNaListViewItem*	item;

	item	= CreateWidget<UNaListViewItem>( GetWorld(), ListItemClass );
	if ( item ){
		item->Setup( this );
		item->SetItem( lvi );
		item->Focused().AddLambda( [this]( UNaListViewItem* payload )
		{
			int32	idx = m_ListItems.IndexOfByKey( payload );

			if ( idx >= 0 ){
				SetCurSel( idx );
			}
		}, item );
		m_ListItems.Add( item );

		m_Panel->AddChild( item );
	}
}

//! 項目上書き
void UNaListView::SetItem( int32 index, const FNaListItem& lvi )
{
	if ( m_ListItems.IsValidIndex( index ) ){
		m_ListItems[index]->SetItem( lvi );
	}
}

//! 
void UNaListView::DeleteItem( int32 index )
{
	m_Panel->RemoveChild( m_ListItems[index] );
	m_ListItems.RemoveAt( index );

	int32	prevIdx = m_CurSel;

	if ( prevIdx >= m_ListItems.Num() ){
		prevIdx--;
	}
	
	m_CurSel	= -1;
	SetCurSel( prevIdx );
}

//!
void UNaListView::DeleteAllItems()
{
	m_Panel->ClearChildren();

	m_ListItems.Reset();
	m_CurSel	= -1;
}

//!
void UNaListView::SetCurSel( int32 index )
{
	UNaListViewItem*	lvi;

	if ( index != m_CurSel ){
		if ( m_ListItems.IsValidIndex( m_CurSel ) ){
			lvi	= m_ListItems[m_CurSel];
			lvi->SelectItem( false );
		}

		m_CurSel	= index;

		if ( m_ListItems.IsValidIndex( m_CurSel ) ){
			lvi	= m_ListItems[m_CurSel];
			lvi->SelectItem( true );
		}
		else {
			m_CurSel	= -1;
		}

		m_SelectedIndexChangedEvent.Broadcast( m_CurSel );
	}
}

//!
void UNaListView::MoveCurSel( int32 value, bool isLoop )
{
	int32	index = m_CurSel;

	if ( index < 0 ){
		index	= 0;
	}
	else {
		int32	add = (value >= 0) ? 1 : -1;

		for ( int32 i = m_ListItems.Num() - 1; i >= 0; --i ){
			index	= FMath::Clamp( index + add, 0, m_ListItems.Num() - 1 );
			if ( m_ListItems[index]->bIsEnabled ){
				break;
			}
		}
	}

	if ( index != m_CurSel ){
		SetCurSel( index );
	}
}

//! 
void UNaListView::OnSlotAdded(UPanelSlot* slot)
{
	if ( MyBox.IsValid() && slot->Content ){
		MyBox->SetContent( slot->Content->TakeWidget() );
	}
}

//! 
void UNaListView::OnSlotRemoved(UPanelSlot* slot)
{
	if ( MyBox.IsValid() ){
		MyBox->SetContent( SNullWidget::NullWidget );

		if ( IsDesignTime() ){
			MyBox->SetContent(
				SNew( SNaBox )
				.HAlign( HAlign_Center )
				.VAlign( VAlign_Center )
				[
					SNew(STextBlock)
					.Text(FText::FromName(GetFName()))
				]
			);
		}
	}
}

//! 
TSharedRef<SWidget> UNaListView::RebuildWidget()
{
	MyBox	= SNew( SNaBox );
	MyBox->m_Owner	= this;

	m_Panel	= nullptr;

	if ( IsDesignTime() ){
		MyBox->SetContent(
			SNew( SNaBox )
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromName(GetFName()))
			]
		);
	}

	// Add any existing content to the new slate box
	if ( GetChildrenCount() > 0 ){
		UPanelSlot* ContentSlot = GetContentSlot();

		if ( ContentSlot->Content ){
			MyBox->SetContent(ContentSlot->Content->TakeWidget());

			m_Panel	= Cast<UPanelWidget>( ContentSlot->Content );
		}
	}

	return MyBox.ToSharedRef();
}
