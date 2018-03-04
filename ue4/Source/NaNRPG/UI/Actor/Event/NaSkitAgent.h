// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Actor/NaUIAgent.h"

#include "UI/Widgets/Event/NaMessageWidget.h"
#include "UI/Widgets/Event/NaSelectionWidget.h"

#include "NaSkitAgent.generated.h"

//! 寸劇系UI仲介アクタ
UCLASS(abstract)
class NANRPG_API ANaSkitAgent : public ANaUIAgent
{
	GENERATED_BODY()

	//! ステート
	enum EState
	{
		//! なし
		None	= -1,
		//! メッセージ表示実行中
		Message,
		//! 選択肢表示実行中
		Selection,
	};
	
public:
	//! コンストラクタ
	ANaSkitAgent( const FObjectInitializer& ObjectInitializer );

	//! 開始
	virtual void	Start( int32 param = 0 ) override;

	//! メッセージ表示
	void	ShowMessage( FText message );
	//! 
	void	HideMessage();

	//! 選択肢表示
	void	ShowSelection( const TArray<FText>& selections, int32 defaultIndex = 0 );
	//! 選択インデックス取得
	int32	GetSelectionIndex();

	//! メッセージ表示完了イベント
	DECLARE_EVENT( ANaSkitAgent, FMessageReachedEvent )
	FMessageReachedEvent&	MessageReached()	{ return m_MessageReachedEvent; }
	//! 選択肢決定イベント
	DECLARE_EVENT_OneParam( ANaSkitAgent, FSelectionResultEvent, int32 )
	FSelectionResultEvent&	SelectionResult()	{ return m_SelectionResultEvent; }

protected:
	//! 更新
	virtual void	OnTick( UNaStateMachine* sm, float DeltaTime ) override;

	//! メッセージ表示
	void	ProcMessage( UNaStateMachine* sm, float DeltaTime );
	//! 選択肢表示
	void	ProcSelection( UNaStateMachine* sm, float DeltaTime );

public:
	//! メッセージウィジェットクラス
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UNaMessageWidget>	MessageWidgetClass;
	//! 選択肢ウィジェットクラス
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UNaSelectionWidget>	SelectionWidgetClass;

protected:
	//! メッセージ表示
	UPROPERTY()
	UNaMessageWidget*	m_MessageWidget;
	//! 選択肢表示
	UPROPERTY()
	UNaSelectionWidget*	m_SelectionWidget;

	//! 
	FText	m_Message;

	//! 
	FMessageReachedEvent	m_MessageReachedEvent;
	//! 
	FSelectionResultEvent	m_SelectionResultEvent;
};
