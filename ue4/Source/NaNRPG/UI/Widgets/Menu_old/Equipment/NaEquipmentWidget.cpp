// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaEquipmentWidget.h"

#include "Entity/Character/NaEntityCharacter.h"

#include "Utility/Components/NaStateMachine.h"

#include "UI/Widgets/Menu_old/Inventory/NaUIItemList.h"

//!
void UNaEquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//!
	m_LvMain	= WidgetTree->FindWidget<UNaListView>( "EquipList" );
	if ( m_LvMain ){
	}
	//! 
	m_ItemView	= WidgetTree->FindWidget<UNaUIItemList>( "ItemList" );
	if ( m_ItemView ){
	}
}

//!
void UNaEquipmentWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );

	if ( m_InvalidateView ){
		UpdateEquipment();
		m_InvalidateView	= false;
	}
}

//! 
FReply UNaEquipmentWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnPreviewKeyDown( InGeometry, InKeyEvent );
}

//!
FReply UNaEquipmentWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if ( !InKeyEvent.IsRepeat() && !m_InvalidateView ){
		const FKey	key = InKeyEvent.GetKey();

		switch ( m_StateMachine->GetState() ){
		case EState::Equipment:
			if ( key == EKeys::Enter ){
				UpdateSelectionItems();

				m_ItemView->SetVisibility( ESlateVisibility::Visible );
				m_ItemView->SetKeyboardFocus();

				m_StateMachine->ChangeState( EState::SelectItem );

				return FReply::Handled();
			}
			else if ( key == EKeys::BackSpace ){
				m_Result	= EUIResult::Canceled;
				return FReply::Handled();
			}
			break;
		case EState::SelectItem:
			if ( key == EKeys::Enter ){
				int32	equipIdx = m_LvMain->GetCurSel();
				int32	itemIdx = m_ItemView->GetCurSel();

				m_Target->EquipItem( equipIdx, m_SelectItems[itemIdx] );
				UpdateEquipment( equipIdx );

				m_LvMain->SetKeyboardFocus();

				m_ItemView->SetVisibility( ESlateVisibility::Hidden );

				m_StateMachine->ChangeState( EState::Equipment );
			}
			else if ( key == EKeys::BackSpace ){
				m_LvMain->SetKeyboardFocus();

				m_ItemView->SetVisibility( ESlateVisibility::Hidden );

				m_StateMachine->ChangeState( EState::Equipment );
			}
			break;
		}
	}

	return Super::NativeOnKeyDown( InGeometry, InKeyEvent );
}

//!
void UNaEquipmentWidget::Open( int32 ZOrder )
{
	StorePrevFocus();

	if ( !IsInViewport() ){
		AddToViewport();
	}
	SetVisibility( ESlateVisibility::Visible );

	SetInputModeUI();
//	m_LvMain->SetKeyboardFocus();

	m_StateMachine->ChangeState( EState::Equipment );

/*	if ( m_UIAction ){
		m_UIAction->DeleteAllItems();

		for ( auto& it : ActionSettings ){
			m_UIAction->AddItem( "",  it.DisplayName.ToString(), it.Icon.ToString() );
		}

		m_UIAction->SetCurSel( 0 );
	}*/

//	m_Result	= EUIResult::None;
}

//!
void UNaEquipmentWidget::Close()
{
	Super::Close();

	UWorld*				world = GetWorld();
	APlayerController*	pc = world->GetFirstPlayerController();

	SetVisibility( ESlateVisibility::Hidden );

	pc->SetInputMode( FInputModeGameOnly() );

	RestorePrevFocus();

	m_Result			= EUIResult::None;
	m_InvalidateView	= true;
}

//! 表示対象セット
void UNaEquipmentWidget::SetCharacter( UNaEntityCharacter* chara )
{
	m_Target			= chara;
	m_InvalidateView	= true;
}

//! 
void UNaEquipmentWidget::UpdateEquipment( int32 index )
{
	if ( m_Target ){
		const TArray<FNaCharacterEquipPart>&	equip = m_Target->GetEquipments();

		if ( index >= 0 ){
			const FNaCharacterEquipPart*	eq = &equip[index];
			FNaListItem	lvi;

			lvi.Value	= EquipPartText[eq->Part].ToString();

			if ( eq->Item ){
				lvi.Params.Add( "itemName", eq->Item->GetDisplayName() );
				lvi.Params.Add( "icon", FString::FromInt( eq->Item->GetIconID() ) );
				lvi.Params.Add( "hsv", eq->Item->GetHSV().ToString() );
			}
			else {
				lvi.Params.Add( "itemName", "EMPTY" );
				lvi.Params.Add( "icon", FString() );
				lvi.Params.Add( "hsv", FString() );
			}
			m_LvMain->SetItem( index, lvi );
		}
		else {
			m_LvMain->DeleteAllItems();

			for ( auto& it : equip ){
				FNaListItem	lvi;

				lvi.Value	= EquipPartText[it.Part].ToString();

				if ( it.Item ){
					lvi.Params.Add( "itemName", it.Item->GetDisplayName() );
					lvi.Params.Add( "icon", FString::FromInt( it.Item->GetIconID() ) );
					lvi.Params.Add( "hsv", it.Item->GetHSV().ToString() );
				}
				else {
					lvi.Params.Add( "itemName", "EMPTY" );
					lvi.Params.Add( "icon", FString() );
					lvi.Params.Add( "hsv", FString() );
				}
				m_LvMain->AddItem( lvi );
			}

			m_LvMain->SetCurSel( 0 );
		}
	}
}

//! 
void UNaEquipmentWidget::UpdateSelectionItems()
{
	m_SelectItems.Reset();
	m_ItemView->DeleteAllItems();

	if ( m_Target ){
		const TArray<FNaCharacterEquipPart>&	equipList = m_Target->GetEquipments();
		int32	idx;

		idx	= m_LvMain->GetCurSel();
		if ( equipList.IsValidIndex( idx ) ){
			const FNaCharacterEquipPart*	equip;

			m_SelectItems.Add( nullptr );
			m_ItemView->AddText( "REMOVE" );

			equip	= &equipList[idx];
			for ( auto& it : m_Target->GetInventory()->m_pItems ){
				if ( it->IsEquip() ){
					continue;
				}
				if ( it->CanEquipment( equip->Part ) ){
					m_SelectItems.Add( it );
					m_ItemView->AddItem( it );
				}
			}

			m_ItemView->SetCurSel( 0 );
		}
	}
}
