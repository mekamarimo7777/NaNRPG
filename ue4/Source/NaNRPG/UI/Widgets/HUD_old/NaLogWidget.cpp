// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaLogWidget.h"

#include "UI/Controls/NaTextBlock.h"

//!
void UNaLogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if ( !m_LvMain ){
		m_LvMain	= Cast<UScrollBox>( WidgetTree->FindWidget( "LogList" ) );
	}
}

//!
void UNaLogWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );
}

//!
FReply UNaLogWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	return Super::NativeOnFocusReceived( InGeometry, InFocusEvent );
}

//!
void UNaLogWidget::AddText( const FString& str, FLinearColor color )
{
	if ( m_LvMain ){
		UNaTextBlock*	tb;

		tb	= WidgetTree->ConstructWidget<UNaTextBlock>( UNaTextBlock::StaticClass() );
		if ( tb ){
			tb->SetFont( FontSetting );
			tb->SetText( FText::FromString(str) );
			tb->SetColorAndOpacity( color );

			if ( m_LvMain->GetChildrenCount() >= 16 ){
				m_LvMain->RemoveChildAt( 0 );
			}
			m_LvMain->AddChild( tb );
			m_LvMain->ScrollWidgetIntoView( tb, false );
		}
	}
}
