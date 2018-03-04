// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "NaHUDMainStatus.generated.h"

class UNaEntityCharacter;

UCLASS()
class NANRPG_API UNaHUDMainStatus : public UNaWidget
{
	GENERATED_BODY()
	
public:
	//!
	virtual void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//! 
	void	BindTarget( UNaEntityCharacter* chara );

	//! 
	UFUNCTION(BlueprintCallable, Category = "HUD")
	FText	GetTargetName() const;
	//! 
	UFUNCTION(BlueprintCallable, Category = "HUD")
	int32	GetHealth() const;
	//! 
	UFUNCTION(BlueprintCallable, Category = "HUD")
	int32	GetMaxHealth() const;

protected:

protected:
	//! 表示ターゲット
	UNaEntityCharacter*	m_Target;
};
