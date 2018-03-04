// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaTextBlock.h"

//!
UNaTextBlock::UNaTextBlock( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer )
{
	AutoWrapText	= true;
}

//!
FReply UNaTextBlock::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	return FReply::Unhandled();
}
