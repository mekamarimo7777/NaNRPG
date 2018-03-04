// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPGEd.h"
#include "NaMapPartsFactory.h"

//
UNaMapPartsFactory::UNaMapPartsFactory( const FObjectInitializer& ObjectInitializer)
: Super( ObjectInitializer )
{
	SupportedClass	= UNaMapPartsAsset::StaticClass();
	bCreateNew		= true;
}

//
bool UNaMapPartsFactory::DoesSupportClass(UClass* Class)
{
	return Class == UNaMapPartsAsset::StaticClass();
}

//
UClass*	UNaMapPartsFactory::ResolveSupportedClass()
{
	return UNaMapPartsAsset::StaticClass();
}

//
UObject* UNaMapPartsFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UNaMapPartsAsset*	newAsset;
	
	newAsset	= NewObject<UNaMapPartsAsset>(InParent, InName, Flags);

	return newAsset;
}
