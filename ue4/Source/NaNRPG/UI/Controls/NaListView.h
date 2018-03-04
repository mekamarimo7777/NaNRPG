// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/Common/NaListViewItem.h"

#include "NaListView.generated.h"

UCLASS(Meta = (DisplayName = "NaListView"))
class NANRPG_API UNaListView : public UContentWidget
{
	GENERATED_BODY()
	
public:	
	//!
	UNaListView( const FObjectInitializer& ObjectInitializer );

/*	//!
	virtual void	NativeConstruct() override;
	//!
	virtual FReply	NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	//!
	virtual void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//!
	virtual FReply	NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;*/
	//!
	virtual FReply	NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent);
	//!
	virtual FReply	NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);

	//! €–Ú’Ç‰Á
	void	AddItem( const FNaListItem& lvi );
	//! €–Úã‘‚«
	void	SetItem( int32 index, const FNaListItem& lvi );
	//!
//	void	InsertListItem( int32 index, const NaListViewItem& lvi );
	//! €–Úíœ
	void	DeleteItem( int32 index );
	//! ‘S€–Úíœ
	void	DeleteAllItems();

	//!
	void	AddSeparator();

	//!
	void	SetCurSel( int32 index );
	//!
	int32	GetCurSel() const		{return m_CurSel;}
	//!
	void	MoveCurSel( int32 value, bool isLoop = true );
	//!
	int32	GetItemCount() const			{return m_ListItems.Num();}

	//!
	FString	GetItemParameter( int32 index, FName key ) const	{ return m_ListItems[index]->GetParameter( key ); }

	// UPanelWidget interface
	virtual void	OnSlotAdded(UPanelSlot* Slot) override;
	virtual void	OnSlotRemoved(UPanelSlot* Slot) override;
	// End of UPanelWidget interface

protected:
	// UWidget interface
	virtual TSharedRef<SWidget>	RebuildWidget() override;
	// End of UWidget interface

public:
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Behavior)
	TSubclassOf<UNaListViewItem>	ListItemClass;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Behavior)
	TSubclassOf<UUserWidget>		SeparatorClass;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Behavior)
	bool							IsFocusable;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Behavior)
	bool							IgnoreKeyEvent;

	//! 
	DECLARE_EVENT_OneParam( UNaUIListView, FSelectedIndexChangedEvent, int32 )
	FSelectedIndexChangedEvent&	SelectedIndexChanged()	{ return m_SelectedIndexChangedEvent; }
	//! 
	DECLARE_EVENT_OneParam( UNaUIListView, FDecidedEvent, int32 )
	FDecidedEvent&				Decided()				{ return m_DecidedEvent; }
	//! 
	DECLARE_EVENT( UNaUIListView, FCanceledEvent )
	FCanceledEvent&				Canceled()				{ return m_CanceledEvent; }
	
protected:
	//! 
	TSharedPtr<class SNaBox>	MyBox;

	//
	UPROPERTY()
	UPanelWidget*				m_Panel;

	//
	UPROPERTY()
	TArray<UNaListViewItem*>	m_ListItems;
	//
	int32						m_CurSel;

	//! events
	FSelectedIndexChangedEvent	m_SelectedIndexChangedEvent;
	//! 
	FDecidedEvent				m_DecidedEvent;
	//! 
	FCanceledEvent				m_CanceledEvent;
};
