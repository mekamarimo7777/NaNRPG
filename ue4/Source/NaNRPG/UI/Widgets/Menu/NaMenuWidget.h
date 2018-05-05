// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "UI/Controls/NaListView.h"

UENUM()
enum class ENaMenuCategory
{
	//! インベントリ
	Inventory,
	//! スキル
	Skill,
	//! アビリティ
	Ability,
	//! 装備
	Equipment,

	ENUM_MAX	UMETA(Hidden)
};

#include "NaMenuWidget.generated.h"

class UNaEntityCharacter;

//! メニュー項目
USTRUCT()
struct FNaMenuItemDefinition
{
	GENERATED_BODY()
	
	//! 表示名
	UPROPERTY(EditAnywhere, Category = "General")
	FText					DisplayName;
	//! アイコンパス
	UPROPERTY(EditAnywhere, Category = "General")
	FStringAssetReference	Icon;
};

UCLASS()
class NANRPG_API UNaMenuWidget : public UNaWidget
{
	GENERATED_BODY()

public:
	//! ステート
	enum EState
	{
		//! なし
		None	= -1,
		//! メイン
		Main,
	};
	
public:	
	//! ウィジェット初期化
	virtual bool	Initialize() override;
	//!
	virtual void	NativeConstruct() override;
	//! Tick
	virtual void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//! キー入力
	virtual FReply	NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	//! ステート設定
	void	SetState( EState state );

	//! メニュー項目追加
	void	AddMenuItem( ENaMenuCategory category );
	//! メニュー項目クリア
	void	ClearMenuItems();

	//! パーティ追加
	void	AddPartyMember( UNaEntityCharacter* chara );
	//! パーティクリア
	void	ClearParty();

	//! 決定イベント
	DECLARE_EVENT_OneParam( UNaMenuWidget, FDecideEvent, int32 )
	FDecideEvent&	Decided()	{ return m_DecidedEvent; }

protected:
	//! メイン
	void	ProcMain( UNaStateMachine* sm, float DeltaTime );
	FReply	ProcMainKeyDown( UNaStateMachine* sm, const FKeyEvent* KeyEvent );

	//! メニューカテゴリ生成
	void	UpdateMenuItems();
	//! パーティリスト生成
	void	UpdatePartyItems();

public:
	UPROPERTY(EditAnywhere, Category = "Definition")
	FNaMenuItemDefinition	MenuItemDefinition[ENaMenuCategory::ENUM_MAX];
	
protected:
	//! メニューリスト
	UPROPERTY()
	UNaListView*	m_LvMenu;
	//! パーティリスト
	UPROPERTY()
	UNaListView*	m_LvParty;

	//! メニュー項目
	TArray<ENaMenuCategory>		m_MenuItems;
	//! 表示キャラクタ
	UPROPERTY()
	TArray<UNaEntityCharacter*>	m_PartyMembers;

	//! イベント
	FDecideEvent	m_DecidedEvent;
};
