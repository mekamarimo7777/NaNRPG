// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"

#include "NaPopupDamage.generated.h"

UCLASS()
class NANRPG_API UNaPopupDamage : public UNaWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category="Binding")
	void	BindingDisplayValue( FText& outVal );

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Damage")
	int32	DisplayValue;
	
public:	
	void	SetDamage( int32 value )	{DisplayValue = value;}

	


};
