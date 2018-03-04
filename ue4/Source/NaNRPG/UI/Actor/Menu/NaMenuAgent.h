// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Actor/NaUIAgent.h"

#include "UI/Widgets/Menu/NaMenuWidget.h"
#include "UI/Widgets/Menu/Inventory/NaInventoryWidget.h"
#include "UI/Widgets/Menu/Equipment/NaEquipWidget.h"

#include "NaMenuAgent.generated.h"

//! メインメニューUI仲介アクタ
UCLASS(abstract)
class NANRPG_API ANaMenuAgent : public ANaUIAgent
{
	GENERATED_BODY()

public:
	//! ステート
	enum EState
	{
		//! なし
		None	= -1,
		//! メインメニュー
		Main,
		//! インベントリ
		Inventory,
		//! 装備
		Equipment,
	};

	//! 結果
	enum EResult
	{
		//! 終了
		Exit,
		//! アイテム選択
		SelectItem,
		//! スキル選択
		SelectSkill,
	};

	//! アイテム選択結果
	struct FItemResult
	{
		//! アクション
		//! アイテムID
	};
	//! スキル選択結果
	struct FSkillResult
	{
	};
	
public:
	//! コンストラクタ
	ANaMenuAgent( const FObjectInitializer& ObjectInitializer );

	//! メニュー操作開始
	virtual void	Start( int32 param ) override;

	//! 結果取得
	EResult	GetResult();
	//! アイテム選択結果詳細
	void	GetItemResult();
	//! スキル選択結果詳細
	void	GetSkillResult();

protected:
	//! 更新
	virtual void	OnTick( UNaStateMachine* sm, float DeltaTime ) override;

	//! メイン
	void	ProcMain( UNaStateMachine* sm, float DeltaTime );
	//! インベントリ
	void	ProcInventory( UNaStateMachine* sm, float DeltaTime );
	//! 装備
	void	ProcEquipment( UNaStateMachine* sm, float DeltaTime );
	//! スキル
	void	ProcSkill( UNaStateMachine* sm );


public:
	//! メインメニューウィジェットクラス
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UNaMenuWidget>		MenuWidgetClass;
	//! インベントリウィジェットクラス
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UNaInventoryWidget>	InventoryWidgetClass;
	//! 装備ウィジェットクラス
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UNaEquipWidget>		EquipWidgetClass;

protected:
	//! メインメニューウィジェット
	UPROPERTY()
	UNaMenuWidget*		m_MenuWidget;
	//! インベントリウィジェット
	UPROPERTY()
	UNaInventoryWidget*	m_InventoryWidget;
	//! 装備ウィジェット
	UPROPERTY()
	UNaEquipWidget*		m_EquipWidget;

	//! 結果
	EResult	m_Result;
};
