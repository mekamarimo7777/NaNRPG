// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "UI/Controls/NaListView.h"

#include "NaLogWidget.generated.h"

//! ログ表示ウィンドウ
UCLASS()
class NANRPG_API UNaLogWidget : public UNaWidget
{
	GENERATED_BODY()
	
public:
	//!
	virtual void	NativeConstruct() override;
	//!
	virtual void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//!
	virtual FReply	NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	//! 
	UFUNCTION(BlueprintCallable, Category = "UI")
	void	AddText( const FString& str, FLinearColor color );

public:
	//! 
	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	FSlateFontInfo	FontSetting;
	
protected:
	//!
	UPROPERTY()
	UScrollBox*	m_LvMain;
	
};
