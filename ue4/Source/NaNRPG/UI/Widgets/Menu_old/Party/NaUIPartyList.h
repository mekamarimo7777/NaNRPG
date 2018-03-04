// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "UI/Controls/NaListView.h"

#include "NaUIPartyList.generated.h"

class UNaEntityCharacter;

//! パーティ表示ウィンドウ
UCLASS()
class NANRPG_API UNaUIPartyList : public UNaWidget
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
	void	SetCurSel( int32 idx )	{ m_LvMain->SetCurSel( idx ); }
	//!
	int32	GetCurSel() const		{ return m_LvMain->GetCurSel(); }
	//!
	void	MoveCurSel( int32 idx, bool isLoop = true )	{ m_LvMain->MoveCurSel( idx, isLoop ); }

	//!
	void	DeleteAllItems();
	//!
	void	AddMember( UNaEntityCharacter* chara );

protected:
	//!
	UPROPERTY()
	UNaListView*	m_LvMain;
	
};
