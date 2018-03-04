// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "Entity/Character/NaEntityCharacter.h"

//#include "NaHUDMainStatus.h"
//#include "NaHUDEnviron.h"
//#include "NaLogWidget.h"

#include "NaHUDWidget.generated.h"

//! HUD表示ウィジェット
UCLASS()
class NANRPG_API UNaHUDWidget : public UNaWidget
{
	GENERATED_BODY()
	
public:	
	//! ウィジェット初期化
	virtual bool	Initialize() override;
	//!
	virtual void	NativeConstruct() override;

	//! キャラクターをバインド
	void	BindTarget( UNaEntityCharacter* chara );
	//! バインド中キャラクター取得
	UFUNCTION(BlueprintCallable, Category = "HUD")
	UNaEntityCharacter*	GetTarget() const	{ return m_Target; }

	//! 
	UFUNCTION(BlueprintCallable, Category = "HUD")
	int32				GetMoney() const	{ return m_Target ? m_Target->GetMoney() : 0; }

	//! ログウィンドウ取得
//	UNaLogWidget*	GetLogWidget()	{ return m_LogWidget; }

public:
	//! 

	
protected:
	//! 表示ターゲット
	UPROPERTY()
	UNaEntityCharacter*	m_Target;

/*	//! メインステータス
	UPROPERTY()
	UNaHUDMainStatus*	m_UIStatus;
	//! 環境表示
	UPROPERTY()
	UNaHUDEnviron*		m_UIEnviron;
	//! ログウィンドウ
	UPROPERTY()
	UNaLogWidget*		m_LogWidget;*/
};
