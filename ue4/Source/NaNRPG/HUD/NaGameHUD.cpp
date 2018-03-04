// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaGameHUD.h"

#include "UMG.h"

//! 
void ANaGameHUD::BeginPlay()
{
	Super::BeginPlay();

	CreateHUD();
}

//!
void ANaGameHUD::Tick( float DeltaTime )
{
#if WITH_EDITOR
	TSharedPtr<SWidget>	widget;

	widget	= FSlateApplication::Get().GetKeyboardFocusedWidget();

	if ( widget.IsValid() ){
		if ( widget->GetType() == "SObjectWidget" ){
			SObjectWidget*	ui = static_cast<SObjectWidget*>( widget.Get() );

			GEngine->AddOnScreenDebugMessage( -1, DeltaTime, FColor::Yellow, FString::Printf( TEXT("FocusWidget: %s"), *ui->GetWidgetObject()->GetName() ) );
		}
		else {
			GEngine->AddOnScreenDebugMessage( -1, DeltaTime, FColor::Yellow, FString::Printf( TEXT("FocusWidget: %s"), *widget->ToString() ) );
		}
	}
#endif
}

//
void ANaGameHUD::CreateHUD()
{
	if ( HUDWidgetClass ){
		m_HUDWidget	= CreateWidget<UNaHUDWidget>( GetWorld(), HUDWidgetClass );
		m_HUDWidget->AddToPlayerScreen();
	}
}
/*
//!
UNaUIMainMenu* ANaGameHUD::GetMainMenu()
{
	if ( !m_pMainMenu ){
		m_pMainMenu	= CreateWidget<UNaUIMainMenu>( GetWorld(), MainMenuClass );
		m_pMainMenu->Initialize();
	}
	return m_pMainMenu;
}

//!
UNaUIInventory* ANaGameHUD::GetInventoryView()
{
	if ( !m_pInventory ){
		m_pInventory	= CreateWidget<UNaUIInventory>( GetWorld(), InventoryClass );
		m_pInventory->Initialize();
	}
	return m_pInventory;
}

//!
UNaEquipmentWidget* ANaGameHUD::GetEquipmentWidget()
{
	if ( !m_EquipWidget ){
		m_EquipWidget	= CreateWidget<UNaEquipmentWidget>( GetWorld(), EquipmentWidgetClass );
		m_EquipWidget->Initialize();
	}
	return m_EquipWidget;
}
*/