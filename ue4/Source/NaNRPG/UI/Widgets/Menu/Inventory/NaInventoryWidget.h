// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widgets/NaWidget.h"


#include "NaInventoryWidget.generated.h"

UCLASS()
class NANRPG_API UNaInventoryWidget : public UNaWidget
{
	GENERATED_BODY()
	
public:	
	//! �E�B�W�F�b�g������
	virtual bool	Initialize() override;
	//!
	virtual void	NativeConstruct() override;

	
protected:
};
