// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "UI/Controls/NaListView.h"

#include "NaUICategoryList.generated.h"

//! 汎用カテゴリウィンドウ
UCLASS()
class NANRPG_API UNaUICategoryList : public UNaWidget
{
	GENERATED_BODY()
	
public:
	//!
	virtual void	NativeConstruct() override;
	//!
	virtual void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//!
	void	AddItem( FString name, FString text, FString iconPath );
	//!
	void	DeleteAllItems();

	//!
	void	SetCurSel( int32 idx )	{ m_LvMain->SetCurSel( idx ); }
	//!
	int32	GetCurSel() const		{ return m_LvMain->GetCurSel(); }
	//!
	void	MoveCurSel( int32 idx, bool isLoop = true )	{ m_LvMain->MoveCurSel( idx, isLoop ); }
	//! 
	FName	GetCategoryName( int32 idx ) const;

	//! 
	DECLARE_EVENT_OneParam( UNaUICategoryList, FCategoryChangedEvent, int32 )
	FCategoryChangedEvent&	CategoryChanged()	{ return m_CategoryChangedEvent; }

public:
	//! アイコン参照パス
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FString			IconPath;

protected:
	//!
	UPROPERTY()
	UNaListView*	m_LvMain;
	
	//! カテゴリ変更イベント
	FCategoryChangedEvent	m_CategoryChangedEvent;
};
