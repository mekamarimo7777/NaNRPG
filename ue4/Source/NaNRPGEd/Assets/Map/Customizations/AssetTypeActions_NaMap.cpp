// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPGEd.h"
#include "AssetTypeActions_NaMap.h"

//!
void FAssetTypeActions_NaMap::OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor )
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for ( auto it = InObjects.CreateConstIterator(); it; ++it )
	{
		UNaMapAsset*	obj = Cast<UNaMapAsset>( *it );

		if ( obj ){
			FNaNRPGEdModule*	module = &FModuleManager::LoadModuleChecked<FNaNRPGEdModule>( "NaNRPGEd" );

			module->CreateMapEditor( Mode, EditWithinLevelEditor, obj );
		}
	}
}
