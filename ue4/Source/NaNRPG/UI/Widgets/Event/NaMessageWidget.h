﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "NaMessageWidget.generated.h"

UCLASS()
class NANRPG_API UNaMessageWidget : public UNaWidget
{
	GENERATED_BODY()

	//! ステート
	enum EState
	{
		//! なし
		None	= -1,
		//! 非表示
		Hide,
		//! 表示メイン
		Show,
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

	//! 表示テキスト設定
	void	SetText( FText text );

	//! 表示テキスト取得
	UFUNCTION( BlueprintCallable, Category = "Bindings" )
	FText	GetText() const	{ return m_Text; }

	//! メッセージ到達イベント
	DECLARE_EVENT( UNaMessageWidget, FSimpleEvent )
	FSimpleEvent&	MessageReached()	{ return m_MessageReachedEvent; }

protected:
	//! 非表示ステート
	void	ProcHide( UNaStateMachine* sm, float DeltaTime );
	//! メイン
	void	ProcShow( UNaStateMachine* sm, float DeltaTime );
	FReply	ProcShowKeyDown( UNaStateMachine* sm, const FKeyEvent* KeyEvent );

	//! トランジションイベント
	virtual bool	OnTransition( FName id ) override;

protected:
	//! 表示テキスト
	FText		m_Text;
	//! キャプション
	FText		m_Caption;
	//! 画像
	UPROPERTY()
	UTexture2D*	m_Image;

	//! イベント
	FSimpleEvent	m_MessageReachedEvent;
};
