// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaItemDataTable.h"



//! シリアライズ：FNaItemProperty
FArchive& operator << ( FArchive &ar, FNaItemProperty& obj )
{
	ar << obj.Factor;
	ar << obj.Intensity;
	ar << obj.Duration;

	ar << obj.Action;
	ar << obj.Effect;
	ar.Serialize( obj.Params, sizeof( obj.Params ) );

	return ar;
}
