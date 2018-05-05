// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "UI/Controls/NaListView.h"

UENUM()
enum class ENaMenuCategory
{
	//! �C���x���g��
	Inventory,
	//! �X�L��
	Skill,
	//! �A�r���e�B
	Ability,
	//! ����
	Equipment,

	ENUM_MAX	UMETA(Hidden)
};

#include "NaMenuWidget.generated.h"

class UNaEntityCharacter;

//! ���j���[����
USTRUCT()
struct FNaMenuItemDefinition
{
	GENERATED_BODY()
	
	//! �\����
	UPROPERTY(EditAnywhere, Category = "General")
	FText					DisplayName;
	//! �A�C�R���p�X
	UPROPERTY(EditAnywhere, Category = "General")
	FStringAssetReference	Icon;
};

UCLASS()
class NANRPG_API UNaMenuWidget : public UNaWidget
{
	GENERATED_BODY()

public:
	//! �X�e�[�g
	enum EState
	{
		//! �Ȃ�
		None	= -1,
		//! ���C��
		Main,
	};
	
public:	
	//! �E�B�W�F�b�g������
	virtual bool	Initialize() override;
	//!
	virtual void	NativeConstruct() override;
	//! Tick
	virtual void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//! �L�[����
	virtual FReply	NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	//! �X�e�[�g�ݒ�
	void	SetState( EState state );

	//! ���j���[���ڒǉ�
	void	AddMenuItem( ENaMenuCategory category );
	//! ���j���[���ڃN���A
	void	ClearMenuItems();

	//! �p�[�e�B�ǉ�
	void	AddPartyMember( UNaEntityCharacter* chara );
	//! �p�[�e�B�N���A
	void	ClearParty();

	//! ����C�x���g
	DECLARE_EVENT_OneParam( UNaMenuWidget, FDecideEvent, int32 )
	FDecideEvent&	Decided()	{ return m_DecidedEvent; }

protected:
	//! ���C��
	void	ProcMain( UNaStateMachine* sm, float DeltaTime );
	FReply	ProcMainKeyDown( UNaStateMachine* sm, const FKeyEvent* KeyEvent );

	//! ���j���[�J�e�S������
	void	UpdateMenuItems();
	//! �p�[�e�B���X�g����
	void	UpdatePartyItems();

public:
	UPROPERTY(EditAnywhere, Category = "Definition")
	FNaMenuItemDefinition	MenuItemDefinition[ENaMenuCategory::ENUM_MAX];
	
protected:
	//! ���j���[���X�g
	UPROPERTY()
	UNaListView*	m_LvMenu;
	//! �p�[�e�B���X�g
	UPROPERTY()
	UNaListView*	m_LvParty;

	//! ���j���[����
	TArray<ENaMenuCategory>		m_MenuItems;
	//! �\���L�����N�^
	UPROPERTY()
	TArray<UNaEntityCharacter*>	m_PartyMembers;

	//! �C�x���g
	FDecideEvent	m_DecidedEvent;
};
