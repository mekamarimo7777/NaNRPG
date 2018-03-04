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
		//! 周囲
		Around,
		//! インベントリ
		Inventory,
		//! パーティリスト
		Party,

		ENUM_MAX	UMETA(Hidden)
	};
}

#include "NaUIInventory.generated.h"

//! アクション情報
USTRUCT()
struct FNaInventoryAction
{
	GENERATED_BODY()

	//! アクショントリガ
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TEnumAsByte<ENaActionTrigger::Type>	Action;
	//! 表示名
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FText					DisplayName;
	//! アイコン
	UPROPERTY(EditDefaultsOnly, Category = "Inventory", Meta =(AllowedClasses = "Texture2D"))
	FStringAssetReference	Icon;
};

//! 処理結果
struct FNaItemActionResult
{
	//! 対象アイテム
	UNaItem*				Item;
	//! アクション種別
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

	//! オープン
	virtual void	Open( int32 ZOrder = 0 ) override;
	//! クローズ
	virtual void	Close() override;

	//! アクションセット
	void	SetAction( ENaActionTrigger::Type action );
	//! モードセット
	void	SetMode( ENaInventoryMode::Type mode );

	//! インベントリから登録
	void	SetInventory( UNaInventory* src );
	//! アイテム追加
	void	AddItem( UNaItem* item );
	//! アイテム削除
	void	RemoveItem( UNaItem* item );

	//! エンティティ追加
	void	AddEntity( UNaEntity* entity );
	//! エンティティ削除
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
	//! アクション設定
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<FNaInventoryAction>	ActionSettings;
	//! 
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FText						ModeText[ENaInventoryMode::ENUM_MAX];

protected:
	//! アクションウィンドウ
	UPROPERTY()
	UNaUICategoryList*	m_UIAction;
	//! メインリスト
	UPROPERTY()
	UNaUIItemList*		m_LvMain;
	//! サブリスト
	UPROPERTY()
	UNaUIItemList*		m_LvSub;

	//! インベントリソース
	UPROPERTY()
	TArray<UNaItem*>	m_Items;
	//! エンティティソース
	UPROPERTY()
	TArray<UNaEntity*>	m_Entities;

	//! アクションインデックス
	int32					m_CurrentAction;
	//! 表示ページタイプ
	ENaInventoryMode::Type	m_Mode;
	//! 表示アイテムリスト（フィルタ付き）
	UPROPERTY()
	TArray<UNaItem*>		m_ShowItems;
	//! 
	bool					m_InvalidatePage;

	//! 
	EUIResult::Type			m_Result;
	//! 
	FNaItemActionResult		m_ActionResult;
};
