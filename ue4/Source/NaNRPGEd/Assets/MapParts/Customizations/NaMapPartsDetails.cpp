// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPGEd.h"
#include "NaMapPartsDetails.h"

#include "Factories/Factory.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailPropertyRow.h"
#include "IDetailCustomNodeBuilder.h"

#include "PropertyHandle.h"
#include "PropertyEditorModule.h"
#include "PropertyCustomizationHelpers.h"

#include "ObjectEditorUtils.h"
#include "AssetData.h"

#define LOCTEXT_NAMESPACE "NaNRPGEd"

//! 
TSharedRef<IDetailCustomization> FNaMapPartsDetails::MakeInstance()
{
	return MakeShareable(new FNaMapPartsDetails());
}

//! 
void FNaMapPartsDetails::CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder)
{
	// Create a category so this is displayed early in the properties
	IDetailCategoryBuilder& MyCategory = DetailBuilder.EditCategory( "CategoryName", FText::FromString( TEXT("Extra info") ), ECategoryPriority::Important );
 
        //You can get properties using the detailbuilder
	//MyProperty= DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(MyClass, MyClassPropertyName));
 
	MyCategory.AddCustomRow( LOCTEXT("Extra info", "Row header name") )
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Extra info", "Custom row header name"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(500)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Extra info", "Custom row content"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		];
}

#undef LOCTEXT_NAMESPACE
