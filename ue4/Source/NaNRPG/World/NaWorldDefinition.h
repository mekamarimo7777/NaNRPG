// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "NaWorldDefinition.generated.h"

// ���^�f�[�^���p��
union FNaWorldBlockMeta
{
	//! 
	uint32	Value;
	//! 
	uint8	Height[4];
};

// �ėp�z�u�u���b�N�\����
struct FNaWorldBlockWork
{
	//! 
	uint8				BlockID;
	//! 
	uint8				BlockParam;
	//! 
	FNaWorldBlockMeta	MetaData;
	//! 
	uint8				Blightness;

	//! 
	uint8	VisibleFace	: 6;
	//! 
	bool	Opaque		: 1;

public:
	//! 
	uint8	GetCenterHeight()
	{
		return (MetaData.Height[0] + MetaData.Height[1] + MetaData.Height[2] + MetaData.Height[3]) >> 2;
	}
};
