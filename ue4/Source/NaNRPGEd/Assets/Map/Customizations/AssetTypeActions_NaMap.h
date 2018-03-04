// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AssetTypeActions_Base.h"

#include "Assets/Map/NaMapAsset.h"

/**
 * 
 */
class NANRPGED_API FAssetTypeActions_NaMap : public FAssetTypeActions_Base
{
	
public:
	// IAssetTypeActions Implementation
	//! 
	virtual FText	GetName() const override			{ return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_NaMap", "NaMap"); }
	virtual FColor	GetTypeColor() const override		{ return FColor(127, 255, 192); }
	virtual UClass*	GetSupportedClass() const override	{ return UNaMapAsset::StaticClass(); }
	virtual uint32	GetCategories() override			{ return EAssetTypeCategories::Basic; }

	virtual void	OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>() ) override;
	// End IAssetTypeActions

protected:

};
