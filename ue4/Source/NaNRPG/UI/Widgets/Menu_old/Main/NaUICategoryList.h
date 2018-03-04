// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "UI/Controls/NaListView.h"

#include "NaUICategoryList.generated.h"

//! �ėp�J�e�S���E�B���h�E
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
	//! �A�C�R���Q�ƃp�X
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FString			IconPath;

protected:
	//!
	UPROPERTY()
	UNaListView*	m_LvMain;
	
	//! �J�e�S���ύX�C�x���g
	FCategoryChangedEvent	m_CategoryChangedEvent;
};
