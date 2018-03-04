// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Assets/MapParts/NaMapPartsEditor.h"

#include "Assets/Map/NaMapEditor.h"

//! 
class FNaNRPGEdModule : public IModuleInterface, public IHasMenuExtensibility, public IHasToolBarExtensibility
{
public:
	//! 
	virtual void StartupModule() override;

	//! 
	virtual void ShutdownModule() override;

	//! 
	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}

	//! 
	virtual TSharedRef<FNaMapPartsEditor>	CreateMapPartsEditor( const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UNaMapPartsAsset* Obj );
	//! 
	virtual TSharedRef<FNaMapEditor>		CreateMapEditor( const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UNaMapAsset* Obj );


	/** Gets the extensibility managers for outside entities to extend static mesh editor's menus and toolbars */
	virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override	{ return MenuExtensibilityManager; }
	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override	{ return ToolBarExtensibilityManager; }

private:

private:
/*	//! 
	void RegisterPropertyTypeCustomizations();
	//! 
	void RegisterObjectCustomizations();

	//! 
	void RegisterCustomClassLayout(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate );

	//! 
	void RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate );*/
private:
	//! 
	//TSet< FName > RegisteredClassNames;
	//! 
	//TSet< FName > RegisteredPropertyTypes;

	//! 
	TSharedPtr<FExtensibilityManager>	MenuExtensibilityManager;
	//! 
	TSharedPtr<FExtensibilityManager>	ToolBarExtensibilityManager;
};
