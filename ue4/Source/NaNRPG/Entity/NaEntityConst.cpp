// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaEntityConst.h"

//! シリアライズ：FName
void SerializeFName( FArchive &ar, FName& obj )
{
	FString	str;

	if ( ar.IsSaving() ){
		str	= obj.ToString();
		ar << str;
	}
	else if ( ar.IsLoading() ){
		ar << str;
		obj	= FName(*str);
	}
}

//! シリアライズ：FNaEntityProfile
FArchive& operator << ( FArchive &ar, FNaEntityProfile& obj )
{
	SerializeFName( ar, obj.AssetID );

	ar << obj.DisplayName;
	SerializeFName( ar, obj.ActorName );
	ar << obj.UniqueName;

	ar << obj.AIType;

	return ar;
}
