// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPGEd.h"
#include "NaMapFactory.h"

//
UNaMapFactory::UNaMapFactory( const FObjectInitializer& ObjectInitializer)
: Super( ObjectInitializer )
{
	SupportedClass	= UNaMapAsset::StaticClass();
	bCreateNew		= true;
}

//
bool UNaMapFactory::DoesSupportClass(UClass* Class)
{
	return Class == UNaMapAsset::StaticClass();
}

//
UClass*	UNaMapFactory::ResolveSupportedClass()
{
	return UNaMapAsset::StaticClass();
}

//
UObject* UNaMapFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UNaMapAsset*	newAsset;
	
	newAsset	= NewObject<UNaMapAsset>(InParent, InName, Flags);

	return newAsset;
}
