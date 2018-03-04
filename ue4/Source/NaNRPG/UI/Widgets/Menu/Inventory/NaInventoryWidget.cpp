// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaInventoryWidget.h"


//! 
bool UNaInventoryWidget::Initialize()
{
	if ( Super::Initialize() ){
		
		return true;
	}
	else {
		return false;
	}
}

//!
void UNaInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
