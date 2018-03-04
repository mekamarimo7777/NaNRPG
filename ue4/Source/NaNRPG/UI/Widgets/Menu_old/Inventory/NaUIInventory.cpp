// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaUIInventory.h"

#include "Entity/NaEntity.h"

//!
void UNaUIInventory::NativeConstruct()
{
	Super::NativeConstruct();

	//!
	m_UIAction	= WidgetTree->FindWidget<UNaUICategoryList>( "Action" );
	if ( m_UIAction ){
		m_UIAction->CategoryChanged().AddUObject( this, &UNaUIInventory::CategoryChangedEventHandler );
	}
	//! 
	if ( !m_LvMain ){
		m_LvMain	= Cast<UNaUIItemList>( WidgetTree->FindWidget( "MainList" ) );
	}
	//! 
	if ( !m_LvSub ){
		m_LvSub		= Cast<UNaUIItemList>( WidgetTree->FindWidget( "SubList" ) );
	}
}

//!
void UNaUIInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );

	if ( m_InvalidatePage ){
		UpdateShowItems();
		UpdatePage();

		m_InvalidatePage	= false;
	}
}

//! 
FReply UNaUIInventory::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if ( !InKeyEvent.IsRepeat() && !m_InvalidatePage ){
		const FKey	key = InKeyEvent.GetKey();

		switch ( m_Mode ){
		case ENaInventoryMode::Around:
			if ( key == EKeys::Right ){
				SetMode( ENaInventoryMode::Inventory );
			}
			break;
		case ENaInventoryMode::Inventory:
			if ( key == EKeys::Left ){
				SetMode( ENaInventoryMode::Around );
			}
			else if ( key == EKeys::Right ){
			}
			break;
		}
	}


	return Super::NativeOnPreviewKeyDown( InGeometry, InKeyEvent );
}

//!
FReply UNaUIInventory::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if ( !InKeyEvent.IsRepeat() && !m_InvalidatePage ){
		const FKey	key = InKeyEvent.GetKey();

		if ( key == EKeys::Enter ){
			Decide();
			return FReply::Handled();
		}
		else if ( key == EKeys::BackSpace ){
			m_Result	= EUIResult::Canceled;
			return FReply::Handled();
		}
		else if ( key == EKeys::A ){
			m_UIAction->MoveCurSel( -1 );
			return FReply::Handled();
		}
		else if ( key == EKeys::D ){
			m_UIAction->MoveCurSel( 1 );
			return FReply::Handled();
		}

		if ( key == EKeys::Tab ){
			return FReply::Handled();
		}
	}

	return Super::NativeOnKeyDown( InGeometry, InKeyEvent );
}

//!
void UNaUIInventory::Open( int32 ZOrder )
{
	StorePrevFocus();

	if ( !IsInViewport() ){
		AddToViewport();
	}
	SetVisibility( ESlateVisibility::Visible );

	SetInputModeUI();
	m_LvMain->SetKeyboardFocus();

	if ( m_UIAction ){
		m_UIAction->DeleteAllItems();

		for ( auto& it : ActionSettings ){
			m_UIAction->AddItem( "",  it.DisplayName.ToString(), it.Icon.ToString() );
		}

		m_UIAction->SetCurSel( 0 );
	}

	m_Result			= EUIResult::None;
	m_InvalidatePage	= true;
}

//!
void UNaUIInventory::Close()
{
	Super::Close();

	UWorld*				world = GetWorld();
	APlayerController*	pc = world->GetFirstPlayerController();

	SetVisibility( ESlateVisibility::Hidden );

	pc->SetInputMode( FInputModeGameOnly() );

	RestorePrevFocus();

	m_Items.Reset();
	m_Entities.Reset();
}

//! 
void UNaUIInventory::SetAction( ENaActionTrigger::Type action )
{
	for ( int32 i = 0; i < ActionSettings.Num(); ++i ){
		if ( ActionSettings[i].Action == action ){
			m_CurrentAction		= i;
			m_InvalidatePage	= true;
			break;
		}
	}
}

//! モードセット
void UNaUIInventory::SetMode( ENaInventoryMode::Type mode )
{
	m_Mode				= mode;
	m_InvalidatePage	= true;

	OnInventoryModeChanged( mode );
}

//!
void UNaUIInventory::SetInventory( UNaInventory* src )
{
	m_Items	= src->m_pItems;

	if ( m_Mode == ENaInventoryMode::Inventory ){
		m_InvalidatePage	= true;
	}
}

//! アイテム追加
void UNaUIInventory::AddItem( UNaItem* item )
{
	m_Items.Add( item );

	if ( m_Mode == ENaInventoryMode::Inventory ){
		m_InvalidatePage	= true;
	}
}

//! アイテム削除
void UNaUIInventory::RemoveItem( UNaItem* item )
{
	m_Items.Remove( item );

	if ( m_Mode == ENaInventoryMode::Inventory ){
		int32	idx = m_ShowItems.IndexOfByKey( item );
		if ( idx >= 0 ){
			m_ShowItems.RemoveAt( idx );
			m_LvMain->DeleteItem( idx );
		}
	}
}

//!
void UNaUIInventory::AddEntity( UNaEntity* entity )
{
	m_Entities.Add( entity );

	if ( m_Mode == ENaInventoryMode::Around ){
		m_InvalidatePage	= true;
	}
}

//!
void UNaUIInventory::RemoveEntity( UNaEntity* entity )
{
	m_Entities.Remove( entity );

	if ( m_Mode == ENaInventoryMode::Around ){
		int32	idx = m_ShowItems.IndexOfByKey( entity->GetItemProperty() );
		if ( idx >= 0 ){
			m_ShowItems.RemoveAt( idx );
			m_LvMain->DeleteItem( idx );
		}
	}
}

//! 
void UNaUIInventory::ClearResult()
{
	m_Result	= EUIResult::None;

	m_ActionResult.Action	= ENaActionTrigger::None;
	m_ActionResult.Item		= nullptr;
}

//! 
void UNaUIInventory::UpdateShowItems()
{
	FNaInventoryAction*	act = &ActionSettings[m_CurrentAction];

	m_ShowItems.Reset();

	switch ( m_Mode ){
	case ENaInventoryMode::Around:
		for ( auto& it : m_Entities ){
			UNaItem*	item = it->GetItemProperty();

			if ( !item ){
				continue;
			}
			if ( act->Action == ENaActionTrigger::None || item->HasAction( act->Action ) ){
				m_ShowItems.Add( item );
			}
		}
		break;

	case ENaInventoryMode::Inventory:
		for ( auto& it : m_Items ){
			if ( act->Action == ENaActionTrigger::None || it->HasAction( act->Action ) ){
				m_ShowItems.Add( it );
			}
		}
		break;
	}
}

//! 
void UNaUIInventory::UpdatePage()
{
	m_LvMain->DeleteAllItems();

	switch ( m_Mode ){
	case ENaInventoryMode::Around:
	case ENaInventoryMode::Inventory:
		for ( auto& it : m_ShowItems ){
			m_LvMain->AddItem( it );
		}
		break;
	case ENaInventoryMode::Party:
		break;
	}

	m_LvMain->SetCurSel( 0 );

	GEngine->ForceGarbageCollection( true );
}

//! 
void UNaUIInventory::Decide()
{
	if ( m_Result == EUIResult::Decided ){
		return;
	}

	int32	idx;

	idx	= m_LvMain->GetCurSel();
	if ( idx >= 0 ){
		ENaActionTrigger::Type	action = ActionSettings[m_CurrentAction].Action;

		if ( action == ENaActionTrigger::None ){
			//! 装備中アイテムは移動不可
			if ( m_ShowItems[idx]->IsEquip() ){
				return;
			}

			switch ( m_Mode ){
			case ENaInventoryMode::Around:
				action	= ENaActionTrigger::Take;
				break;
			case ENaInventoryMode::Inventory:
				action	= ENaActionTrigger::Drop;
				break;
			}
		}

		m_ActionResult.Action	= action;
		m_ActionResult.Item		= m_ShowItems[idx];

		m_Result	= EUIResult::Decided;
	}
}

//! 
void UNaUIInventory::CategoryChangedEventHandler( int32 index )
{
	m_CurrentAction		= index;
	m_InvalidatePage	= true;
}
