// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaHUDWidget.h"

//! 
bool UNaHUDWidget::Initialize()
{
	if ( Super::Initialize() ){
		return true;
	}
	else {
		return false;
	}
}

//!
void UNaHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//!
/*	m_UIStatus	= WidgetTree->FindWidget<UNaHUDMainStatus>( "Status" );
	if ( m_UIStatus ){
	}
	//!
	m_LogWidget	= WidgetTree->FindWidget<UNaLogWidget>( "LogWidget" );
	if ( m_LogWidget ){
	}*/
}

//!
void UNaHUDWidget::BindTarget( UNaEntityCharacter* chara )
{
	m_Target	= chara;

//	if ( m_UIStatus ){
//		m_UIStatus->BindTarget( chara );
//	}
}
