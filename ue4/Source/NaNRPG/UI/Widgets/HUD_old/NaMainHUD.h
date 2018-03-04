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

	//! ���O�E�B���h�E�擾
	UNaLogWidget*	GetLogWidget()	{ return m_LogWidget; }
	
protected:
	//! �\���^�[�Q�b�g
	UPROPERTY()
	UNaEntityCharacter*	m_Target;

	//! ���C���X�e�[�^�X
	UPROPERTY()
	UNaHUDMainStatus*	m_UIStatus;
	//! ���\��
	UPROPERTY()
	UNaHUDEnviron*		m_UIEnviron;
	//! ���O�E�B���h�E
	UPROPERTY()
	UNaLogWidget*		m_LogWidget;
};
