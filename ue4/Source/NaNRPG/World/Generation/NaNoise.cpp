﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaNoise.h"

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//
FNaNoise::FNaNoise( int32 seed )
{
	reseed( seed );
}
