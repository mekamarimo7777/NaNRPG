// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "NaListViewItem.generated.h"

class UNaListView;

//! 汎用リストアイテム（データ仲介用）
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

//! リストアイテムUI基底
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

	//! 初期化
	void	Setup( UNaListView* owner );

	//! アイテムデータ設定
	void			SetItem( const FNaListItem& item );
	//! アイテムデータ取得
	FNaListItem*	GetItem();
	//!
	void			SelectItem( bool select );

	//! 値セット
	UFUNCTION(BlueprintCallable, Category = "ListView")
	void		SetValue( FString value );
	//! 値取得
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
	//! オーナーリスト
	UNaListView*	m_Owner;
	//! 選択状態（所属リストの選択対象）
	bool			m_IsSelected;

	//! チェック状態 //
	bool			m_IsChecked;

	//!
	FFocusedEvent	m_FocusedEvent;
};
