// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"

#include "UI/Widgets/HUD/NaHUDWidget.h"

#include "UI/Widgets/HUD_old/NaMainHUD.h"
#include "UI/Widgets/Menu_old/Main/NaUIMainMenu.h"
#include "UI/Widgets/Menu_old/Inventory/NaUIInventory.h"
#include "UI/Widgets/Menu_old/Equipment/NaEquipmentWidget.h"

#include "NaGameHUD.generated.h"

//! HUD���
UCLASS()
class NANRPG_API ANaGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	//! 
	virtual void	BeginPlay() override;
	//! 
	virtual void	Tick(float DeltaTime) override;

	//! HUD�E�B�W�F�b�g�擾
	UNaHUDWidget*	GetHUDWidget() const	{ return m_HUDWidget; }

protected:
	//! HUD����
	void	CreateHUD();

public:
	//! 
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UNaHUDWidget>		HUDWidgetClass;

protected:
	UPROPERTY()
	UNaHUDWidget*	m_HUDWidget;
};
