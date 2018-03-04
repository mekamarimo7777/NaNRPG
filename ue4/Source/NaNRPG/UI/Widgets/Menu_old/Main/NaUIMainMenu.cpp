// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaUIMainMenu.h"

#include "Database/NaGameDatabase.h"

//!
void UNaUIMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	//!
	m_UICategory	= WidgetTree->FindWidget<UNaUICategoryList>( "Category" );
	if ( m_UICategory ){
	}

	//!
	m_UIParty		= WidgetTree->FindWidget<UNaUIPartyList>( "Party" );
	if ( m_UIParty ){
	}
}

//!
void UNaUIMainMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );
}

//! 
FReply UNaUIMainMenu::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey	key = InKeyEvent.GetKey();

	if ( key == EKeys::Left ){
		m_UICategory->MoveCurSel( -1 );
		return FReply::Handled();
	}
	else if ( key == EKeys::Right ){
		m_UICategory->MoveCurSel( 1 );
		return FReply::Handled();
	}

	return Super::NativeOnPreviewKeyDown( InGeometry, InKeyEvent );
}

//!
FReply UNaUIMainMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey	key = InKeyEvent.GetKey();

	if ( key == EKeys::Enter ){
		m_Result	= EUIResult::Decided;
		return FReply::Handled();
	}
	else if ( key == EKeys::BackSpace ){
		m_Result	= EUIResult::Canceled;
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown( InGeometry, InKeyEvent );
}

//!
void UNaUIMainMenu::Open( int32 ZOrder )
{
	Super::Open();

	StorePrevFocus();

	SetVisibility( ESlateVisibility::Visible );

	SetInputModeUI();
	m_UIParty->SetKeyboardFocus();

	if ( m_UICategory ){
		m_UICategory->DeleteAllItems();

		m_UICategory->AddItem( "inventory", TEXT("インベントリ"), "/Game/Textures/UI/icon_inventory.icon_inventory" );
		m_UICategory->AddItem( "arts", TEXT("アビリティ"), "/Game/Textures/UI/icon_arts.icon_arts" );
		m_UICategory->AddItem( "spell", TEXT("スペル"), "/Game/Textures/UI/icon_spell.icon_spell" );
		m_UICategory->AddItem( "equip", TEXT("装備"), "/Game/Textures/UI/icon_equip.icon_equip" );
		m_UICategory->AddItem( "status", TEXT("ステータス"), "/Game/Textures/UI/icon_status.icon_status" );
		m_UICategory->AddItem( "skill", TEXT("スキル"), "/Game/Textures/UI/icon_skill.icon_skill" );
		m_UICategory->AddItem( "option", TEXT("設定"), "/Game/Textures/UI/icon_option.icon_option" );

		m_UICategory->SetCurSel( 0 );
	}
	if ( m_UIParty ){
		UNaGameDatabase*	db = UNaGameDatabase::GetDB();

		m_UIParty->DeleteAllItems();

		// プレイヤー追加
		m_UIParty->AddMember( db->GetPlayer() );

		//@test
		for ( int32 i = 0; i < 16; ++i ){
			m_UIParty->AddMember( db->GetPlayer() );
		}

		m_UIParty->SetCurSel( 0 );
	}

	m_Result	= EUIResult::None;
}

//!
void UNaUIMainMenu::Close()
{
	Super::Close();

	UWorld*				world = GetWorld();
	APlayerController*	pc = world->GetFirstPlayerController();

	SetVisibility( ESlateVisibility::Hidden );
//	RemoveFromParent();

	RestorePrevFocus();
}

//! 
FName UNaUIMainMenu::GetCategory() const
{
	if ( m_UICategory ){
		int32	idx = m_UICategory->GetCurSel();
		return m_UICategory->GetCategoryName( idx );
	}
	else {
		return FName();
	}
}

//! 
int32 UNaUIMainMenu::GetPartyIndex() const
{
	if ( m_UIParty ){
		return m_UIParty->GetCurSel();
	}
	else {
		return -1;
	}
}
