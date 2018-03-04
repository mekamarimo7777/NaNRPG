// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "NaListViewItem.generated.h"

class UNaListView;

//! �ėp���X�g�A�C�e���i�f�[�^����p�j
USTRUCT(BlueprintType)
struct FNaListItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ListView)
	FString					Value;

	UPROPERTY()
	TMap<FName, FString>	Params;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ListView)
	FString					Tag;
};

//! ���X�g�A�C�e��UI���
UCLASS()
class NANRPG_API UNaListViewItem : public UNaWidget
{
	GENERATED_BODY()
	
public:	
	//!
	virtual void	NativeConstruct() override;
	//!
	virtual FReply	NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	//!
	virtual FReply	NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	//! ������
	void	Setup( UNaListView* owner );

	//! �A�C�e���f�[�^�ݒ�
	void			SetItem( const FNaListItem& item );
	//! �A�C�e���f�[�^�擾
	FNaListItem*	GetItem();
	//!
	void			SelectItem( bool select );

	//! �l�Z�b�g
	UFUNCTION(BlueprintCallable, Category = "ListView")
	void		SetValue( FString value );
	//! �l�擾
	UFUNCTION(BlueprintCallable, Category = "ListView")
	FString		GetValue() const;

	//!
	UFUNCTION(BlueprintCallable, Category = "ListView")
	bool	IsSelected() const	{ return m_IsSelected; }

	//!
	DECLARE_EVENT( UNaListViewItem, FFocusedEvent );
	FFocusedEvent&	Focused()	{ return m_FocusedEvent; }

protected:
	//! 
	UFUNCTION(BlueprintImplementableEvent, Category = "ListView")
	void	OnItemSelected(bool selected);

public:
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ListView")
	FNaListItem	ItemValue;

protected:
	//! �I�[�i�[���X�g
	UNaListView*	m_Owner;
	//! �I����ԁi�������X�g�̑I��Ώہj
	bool			m_IsSelected;

	//! �`�F�b�N��� //
	bool			m_IsChecked;

	//!
	FFocusedEvent	m_FocusedEvent;
};
