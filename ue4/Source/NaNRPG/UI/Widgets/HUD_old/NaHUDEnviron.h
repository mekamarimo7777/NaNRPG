// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "UI/Controls/NaListView.h"

#include "NaHUDEnviron.generated.h"

//! 環境表示ウィンドウ
UCLASS()
class NANRPG_API UNaHUDEnviron : public UNaWidget
{
	GENERATED_BODY()
	
public:
	//!
	virtual void	NativeConstruct() override;
	//!
	virtual void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//!
	virtual FReply	NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;


public:
	
protected:
	
};
