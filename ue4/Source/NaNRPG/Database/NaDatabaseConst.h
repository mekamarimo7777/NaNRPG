// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NaDatabaseConst.generated.h"

/**
 * ���[���h�o�^���
 */
USTRUCT()
struct NANRPG_API FNaWorldRecord
{
	GENERATED_BODY()

	//! �ŗLID
	UPROPERTY()
	FName	UID;
	//! �f�[�^ID�i�ۑ��t�H���_�̎��ʗp�B�����_���ɐ����j
	UPROPERTY()
	uint32	DataID;
};
