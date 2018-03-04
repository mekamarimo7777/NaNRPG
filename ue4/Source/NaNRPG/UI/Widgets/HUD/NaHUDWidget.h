// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "Entity/Character/NaEntityCharacter.h"

//#include "NaHUDMainStatus.h"
//#include "NaHUDEnviron.h"
//#include "NaLogWidget.h"

#include "NaHUDWidget.generated.h"

//! HUD�\���E�B�W�F�b�g
UCLASS()
class NANRPG_API UNaHUDWidget : public UNaWidget
{
	GENERATED_BODY()
	
public:	
	//! �E�B�W�F�b�g������
	virtual bool	Initialize() override;
	//!
	virtual void	NativeConstruct() override;

	//! �L�����N�^�[���o�C���h
	void	BindTarget( UNaEntityCharacter* chara );
	//! �o�C���h���L�����N�^�[�擾
	UFUNCTION(BlueprintCallable, Category = "HUD")
	UNaEntityCharacter*	GetTarget() const	{ return m_Target; }

	//! 
	UFUNCTION(BlueprintCallable, Category = "HUD")
	int32				GetMoney() const	{ return m_Target ? m_Target->GetMoney() : 0; }

	//! ���O�E�B���h�E�擾
//	UNaLogWidget*	GetLogWidget()	{ return m_LogWidget; }

public:
	//! 

	
protected:
	//! �\���^�[�Q�b�g
	UPROPERTY()
	UNaEntityCharacter*	m_Target;

/*	//! ���C���X�e�[�^�X
	UPROPERTY()
	UNaHUDMainStatus*	m_UIStatus;
	//! ���\��
	UPROPERTY()
	UNaHUDEnviron*		m_UIEnviron;
	//! ���O�E�B���h�E
	UPROPERTY()
	UNaLogWidget*		m_LogWidget;*/
};
