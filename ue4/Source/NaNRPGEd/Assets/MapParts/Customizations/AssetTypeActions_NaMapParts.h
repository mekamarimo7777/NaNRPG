// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AssetTypeActions_Base.h"

#include "Assets/World/NaMapPartsAsset.h"

/**
 * 
 */
class NANRPGED_API FAssetTypeActions_NaMapParts : public FAssetTypeActions_Base
{
	
public:
	// IAssetTypeActions Implementation
	//! 
	virtual FText	GetName() const override			{ return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_NaMapParts", "NaMapParts"); }
	virtual FColor	GetTypeColor() const override		{ return FColor(0, 255, 255); }
	virtual UClass*	GetSupportedClass() const override	{ return UNaMapPartsAsset::StaticClass(); }
	virtual uint32	GetCategories() override			{ return EAssetTypeCategories::Basic; }

	virtual void	OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>() ) override;
	// End IAssetTypeActions

protected:

};
