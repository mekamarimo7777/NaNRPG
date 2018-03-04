// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "Entity/Character/NaEntityCharacter.h"

#include "NaHUDMainStatus.h"
#include "NaHUDEnviron.h"
#include "NaLogWidget.h"

#include "NaMainHUD.generated.h"

UCLASS()
class NANRPG_API UNaMainHUD : public UNaWidget
{
	GENERATED_BODY()
	
public:	
	//!
	virtual void	NativeConstruct() override;

	//!
	void	BindTarget( UNaEntityCharacter* chara );

	//! 
	UFUNCTION(BlueprintCallable, Category = "HUD")
	UNaEntityCharacter*	GetTarget() const	{ return m_Target; }
	//! 
	UFUNCTION(BlueprintCallable, Category = "HUD")
	int32				GetMoney() const	{ return m_Target ? m_Target->GetMoney() : 0; }

	//! ログウィンドウ取得
	UNaLogWidget*	GetLogWidget()	{ return m_LogWidget; }
	
protected:
	//! 表示ターゲット
	UPROPERTY()
	UNaEntityCharacter*	m_Target;

	//! メインステータス
	UPROPERTY()
	UNaHUDMainStatus*	m_UIStatus;
	//! 環境表示
	UPROPERTY()
	UNaHUDEnviron*		m_UIEnviron;
	//! ログウィンドウ
	UPROPERTY()
	UNaLogWidget*		m_LogWidget;
};
