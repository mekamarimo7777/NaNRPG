// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPGEd.h"
#include "AssetTypeActions_NaMapParts.h"

//!
void FAssetTypeActions_NaMapParts::OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor )
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for ( auto it = InObjects.CreateConstIterator(); it; ++it )
	{
		UNaMapPartsAsset*	obj = Cast<UNaMapPartsAsset>( *it );

		if ( obj ){
			FNaNRPGEdModule*	module = &FModuleManager::LoadModuleChecked<FNaNRPGEdModule>( "NaNRPGEd" );

			module->CreateMapPartsEditor( Mode, EditWithinLevelEditor, obj );
		}
	}
}
