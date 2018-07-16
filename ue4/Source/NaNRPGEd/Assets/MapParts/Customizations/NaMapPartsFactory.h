// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Factories/Factory.h"

#include "Assets/World/NaMapPartsAsset.h"

#include "NaMapPartsFactory.generated.h"

/**
 * 
 */
UCLASS()
class NANRPGED_API UNaMapPartsFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	//
	UNaMapPartsFactory(const FObjectInitializer& ObjectInitializer);

	//
	virtual bool	DoesSupportClass(UClass* Class) override;
	//
	virtual UClass*	ResolveSupportedClass() override;
	//
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

protected:

};
