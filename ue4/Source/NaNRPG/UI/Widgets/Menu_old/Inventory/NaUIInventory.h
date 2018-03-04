// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "Database/NaInventory.h"

#include "UI/Widgets/Menu_old/Main/NaUICategoryList.h"
#include "UI/Widgets/Menu_old/Inventory/NaUIItemList.h"

//! 
UENUM()
namespace ENaInventoryMode
{
	enum Type
	{
		//! ����
		Around,
		//! �C���x���g��
		Inventory,
		//! �p�[�e�B���X�g
		Party,

		ENUM_MAX	UMETA(Hidden)
	};
}

#include "NaUIInventory.generated.h"

//! �A�N�V�������
USTRUCT()
struct FNaInventoryAction
{
	GENERATED_BODY()

	//! �A�N�V�����g���K
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TEnumAsByte<ENaActionTrigger::Type>	Action;
	//! �\����
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FText					DisplayName;
	//! �A�C�R��
	UPROPERTY(EditDefaultsOnly, Category = "Inventory", Meta =(AllowedClasses = "Texture2D"))
	FStringAssetReference	Icon;
};

//! ��������
struct FNaItemActionResult
{
	//! �ΏۃA�C�e��
	UNaItem*				Item;
	//! �A�N�V�������
	ENaActionTrigger::Type	Action;
};

UCLASS()
class NANRPG_API UNaUIInventory : public UNaWidget
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

	//! �I�[�v��
	virtual void	Open( int32 ZOrder = 0 ) override;
	//! �N���[�Y
	virtual void	Close() override;

	//! �A�N�V�����Z�b�g
	void	SetAction( ENaActionTrigger::Type action );
	//! ���[�h�Z�b�g
	void	SetMode( ENaInventoryMode::Type mode );

	//! �C���x���g������o�^
	void	SetInventory( UNaInventory* src );
	//! �A�C�e���ǉ�
	void	AddItem( UNaItem* item );
	//! �A�C�e���폜
	void	RemoveItem( UNaItem* item );

	//! �G���e�B�e�B�ǉ�
	void	AddEntity( UNaEntity* entity );
	//! �G���e�B�e�B�폜
	void	RemoveEntity( UNaEntity* entity );
	
	//!
	EUIResult::Type		GetResult() const	{ return m_Result; }
	//! 
	FNaItemActionResult	GetActionResult()	{ return m_ActionResult; }
	//! 
	void				ClearResult();

	//! 
	UFUNCTION(BlueprintCallable, Category = "Bindings")
	FText	GetModeText() const	{ return ModeText[m_Mode]; }

protected:
	//! 
	void	UpdateShowItems();
	//! 
	void	UpdatePage();
	//! 
	void	Decide();

	//! 
	void	CategoryChangedEventHandler( int32 index );

	//! 
	UFUNCTION(BlueprintImplementableEvent, Category = "Event")
	void	OnInventoryModeChanged( ENaInventoryMode::Type type );

public:
	//! �A�N�V�����ݒ�
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<FNaInventoryAction>	ActionSettings;
	//! 
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FText						ModeText[ENaInventoryMode::ENUM_MAX];

protected:
	//! �A�N�V�����E�B���h�E
	UPROPERTY()
	UNaUICategoryList*	m_UIAction;
	//! ���C�����X�g
	UPROPERTY()
	UNaUIItemList*		m_LvMain;
	//! �T�u���X�g
	UPROPERTY()
	UNaUIItemList*		m_LvSub;

	//! �C���x���g���\�[�X
	UPROPERTY()
	TArray<UNaItem*>	m_Items;
	//! �G���e�B�e�B�\�[�X
	UPROPERTY()
	TArray<UNaEntity*>	m_Entities;

	//! �A�N�V�����C���f�b�N�X
	int32					m_CurrentAction;
	//! �\���y�[�W�^�C�v
	ENaInventoryMode::Type	m_Mode;
	//! �\���A�C�e�����X�g�i�t�B���^�t���j
	UPROPERTY()
	TArray<UNaItem*>		m_ShowItems;
	//! 
	bool					m_InvalidatePage;

	//! 
	EUIResult::Type			m_Result;
	//! 
	FNaItemActionResult		m_ActionResult;
};
