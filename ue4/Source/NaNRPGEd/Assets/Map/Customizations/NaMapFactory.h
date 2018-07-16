// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Factories/Factory.h"

#include "Assets/World/NaMapAsset.h"

#include "NaMapFactory.generated.h"

/**
 * 
 */
UCLASS()
class NANRPGED_API UNaMapFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	//
	UNaMapFactory(const FObjectInitializer& ObjectInitializer);

	//
	virtual bool	DoesSupportClass(UClass* Class) override;
	//
	virtual UClass*	ResolveSupportedClass() override;
	//
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

protected:

};
