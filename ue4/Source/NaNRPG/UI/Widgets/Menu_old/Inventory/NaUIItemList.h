// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "UI/Controls/NaListView.h"

#include "NaUIItemList.generated.h"

class UNaItem;

//! アイテム表示ウィンドウ
UCLASS()
class NANRPG_API UNaUIItemList : public UNaWidget
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
	void	AddText( FString text );
	//!
	void	AddItem( UNaItem* item );
	//! 
	void	DeleteItem( int32 index );
	//!
//	void	AddCharacter( UNaItem* item );

protected:
	//!
	UPROPERTY()
	UNaListView*	m_LvMain;
	
};
