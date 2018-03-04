// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "Database/NaInventory.h"

#include "UI/Widgets/Menu_old/Inventory/NaUIItemList.h"

#include "NaEquipmentWidget.generated.h"

class UNaEntityCharacter;
class UNaStateMachine;

UCLASS()
class NANRPG_API UNaEquipmentWidget : public UNaWidget
{
	GENERATED_BODY()

	enum EState
	{
		//! 
		None,
		//! 
		Equipment,
		//! 
		SelectItem
	};
	
public:	
	//!
	virtual void	NativeConstruct() override;
	//!
	virtual void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//! 
	virtual FReply	NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	//! 
	virtual FReply	NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	//! �I�[�v��
	virtual void	Open( int32 ZOrder = 0 ) override;
	//! �N���[�Y
	virtual void	Close() override;

	//! �\���ΏۃZ�b�g
	void	SetCharacter( UNaEntityCharacter* chara );

	//!
	EUIResult::Type	GetResult() const	{ return m_Result; }

protected:
	//! 
	void	UpdateEquipment( int32 index = -1 );
	//! 
	void	UpdateSelectionItems();

public:
	//! 
	UPROPERTY(EditDefaultsOnly, Category = "Text")
	FText	EquipPartText[ENaItemEquipPart::ENUM_MAX];

protected:
	//! �������X�g
	UPROPERTY()
	UNaListView*	m_LvMain;
	//! �����A�C�e���\�����X�g
	UPROPERTY()
	UNaUIItemList*	m_ItemView;

	//! 
	UPROPERTY()
	UNaEntityCharacter*	m_Target;
	//!
	bool				m_InvalidateView;

	//! 
	UPROPERTY()
	TArray<UNaItem*>	m_SelectItems;

	//! 
	EUIResult::Type		m_Result;
};
