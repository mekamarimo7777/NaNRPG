// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "NaCanvasWidget.generated.h"

//! オーナードロー用テストウィジェット
UCLASS()
class NANRPG_API UNaCanvasWidget : public UNaWidget
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
	//! オーナードロー
	virtual void	NativePaint( FPaintContext & InContext ) const override;

protected:
	//! 非表示ステート
	void	ProcHide( UNaStateMachine* sm, float DeltaTime );
	//! メイン
	void	ProcShow( UNaStateMachine* sm, float DeltaTime );

	//! トランジションイベント
	virtual bool	OnTransition( FName id ) override;

protected:
};
