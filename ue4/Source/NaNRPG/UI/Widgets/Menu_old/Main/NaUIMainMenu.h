// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "UI/Widgets/Menu_old/Main/NaUICategoryList.h"
#include "UI/Widgets/Menu_old/Party/NaUIPartyList.h"

#include "NaUIMainMenu.generated.h"

UCLASS()
class NANRPG_API UNaUIMainMenu : public UNaWidget
{
	GENERATED_BODY()
	
public:
	//!
	virtual void	NativeConstruct() override;
	//!
	virtual void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//! 
	virtual FReply	NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	//! 
	virtual FReply	NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	//!
	virtual void	Open( int32 ZOrder = 0 ) override;
	//!
	virtual void	Close() override;

	//!
	EUIResult::Type	GetResult() const	{ return m_Result; }
	//! 
	FName			GetCategory() const;
	//! 
	int32			GetPartyIndex() const;

protected:
	//! メニューウィンドウ
	UPROPERTY()
	UNaUICategoryList*	m_UICategory;
	//! パーティウィンドウ（小）
	UPROPERTY()
	UNaUIPartyList*		m_UIParty;

	//!
	EUIResult::Type		m_Result;
};
