// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaEquipWidget.h"


//! 
bool UNaEquipWidget::Initialize()
{
	if ( Super::Initialize() ){
		return true;
	}
	else {
		return false;
	}
}

//!
void UNaEquipWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
