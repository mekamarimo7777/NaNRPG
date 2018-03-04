// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPGEd.h"

#include "PropertyEditorModule.h"

#include "AssetToolsModule.h"

#include "Assets/MapParts/Customizations/NaMapPartsDetails.h"
#include "Assets/MapParts/Customizations/AssetTypeActions_NaMapParts.h"

#include "Assets/Map/Customizations/NaMapDetails.h"
#include "Assets/Map/Customizations/AssetTypeActions_NaMap.h"

IMPLEMENT_GAME_MODULE( FNaNRPGEdModule, NaNRPGEd );

//! 
void FNaNRPGEdModule::StartupModule()
{
	if ( FModuleManager::Get().IsModuleLoaded("PropertyEditor") ){
		FPropertyEditorModule&	PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

		//! 
		PropertyModule.RegisterCustomClassLayout( "NaMapParts", FOnGetDetailCustomizationInstance::CreateStatic( &FNaMapPartsDetails::MakeInstance ) );
		PropertyModule.RegisterCustomClassLayout( "NaMap", FOnGetDetailCustomizationInstance::CreateStatic( &FNaMapDetails::MakeInstance ) );
	}

	//! 
	if ( FModuleManager::Get().IsModuleLoaded("AssetTools") ){
		FAssetToolsModule&	AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

		AssetToolsModule.Get().RegisterAssetTypeActions( MakeShareable( new  FAssetTypeActions_NaMapParts ) );
		AssetToolsModule.Get().RegisterAssetTypeActions( MakeShareable( new  FAssetTypeActions_NaMap ) );
	}

	MenuExtensibilityManager	= MakeShareable(new FExtensibilityManager);
	ToolBarExtensibilityManager	= MakeShareable(new FExtensibilityManager);
}

//! 
void FNaNRPGEdModule::ShutdownModule()
{
	if ( FModuleManager::Get().IsModuleLoaded("PropertyEditor") ){
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

		//! 
		PropertyModule.UnregisterCustomClassLayout( "NaMapParts" );
		PropertyModule.UnregisterCustomClassLayout( "NaMap" );
	}

	if ( FModuleManager::Get().IsModuleLoaded("AssetTools") ){
		FAssetToolsModule&	AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

//		AssetToolsModule.Get().UnregisterAssetTypeActions( FAssetTypeActions_NaMapParts.ToSharedRef() );
	}

	MenuExtensibilityManager.Reset();
	ToolBarExtensibilityManager.Reset();
}

//! 
TSharedRef<FNaMapPartsEditor> FNaNRPGEdModule::CreateMapPartsEditor( const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UNaMapPartsAsset* Obj )
{
	TSharedRef<FNaMapPartsEditor>	editor( new FNaMapPartsEditor() );

	editor->InitEditor( Mode, InitToolkitHost, Obj );

	return editor;
}

//! 
TSharedRef<FNaMapEditor> FNaNRPGEdModule::CreateMapEditor( const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UNaMapAsset* Obj )
{
	TSharedRef<FNaMapEditor>	editor( new FNaMapEditor() );

	editor->InitEditor( Mode, InitToolkitHost, Obj );

	return editor;
}
