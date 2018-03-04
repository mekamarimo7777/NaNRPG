// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaTextBlock.generated.h"

UCLASS(Meta = (DisplayName = "NaTextBlock"))
class NANRPG_API UNaTextBlock : public UTextBlock
{
	GENERATED_BODY()
	
public:	
	//!
	UNaTextBlock( const FObjectInitializer& ObjectInitializer );

	//!
	virtual FReply	NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent);

protected:

public:
	
protected:
};
