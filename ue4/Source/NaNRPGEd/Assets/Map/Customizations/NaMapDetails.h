// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PropertyEditorModule.h"

#include "IDetailCustomization.h"

#include "Assets/World/NaMapAsset.h"

/**
 * 
 */
class NANRPGED_API FNaMapDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	// IDetailCustomization interface
	virtual void CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder) override;
	// End of IDetailCustomization interface

protected:

};
