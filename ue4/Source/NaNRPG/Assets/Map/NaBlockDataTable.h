// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "Misc/NaStructs.h"

UENUM()
namespace ENaBlockPhase
{
	enum Type
	{
		//! �ő�
		Solid,
		//! �t��
		Liquid,
		//! �C��
		Gas,
		//! �v���Y�}
		Plasma,

		ENUM_MAX	UMETA(Hidden)
	};
}

#include "NaBlockDataTable.generated.h"

// �}�b�v�}�e���A����` //
USTRUCT()
struct FNaBlockMaterialAsset : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = General)
	int32	UID;
	UPROPERTY(EditDefaultsOnly, Category = General)
	FName	Name;

	UPROPERTY(EditDefaultsOnly, Category = Rendering, Meta = (AllowedClasses = "MaterialInterface"))
	FStringAssetReference	Material;
};

// �}�b�v�u���b�N��` //
USTRUCT()
struct FNaBlockDataAsset : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	//! ID�i0�`254�܂ŁB255�ȏ�͖����j
	UPROPERTY(EditDefaultsOnly, Category = General)
	int32	UID;
	UPROPERTY(EditDefaultsOnly, Category = General)
	FName	Name;
	UPROPERTY(EditDefaultsOnly, Category = General)
	FText	DisplayName;

	//! �`��̗L��
	UPROPERTY(EditDefaultsOnly, Category = Rendering)
	bool	IsRender;
	//! ���b�V���`��
	UPROPERTY(EditDefaultsOnly, Category = Rendering)
	int32	MeshType;
	//! �}�b�v�}�e���A��ID
	UPROPERTY(EditDefaultsOnly, Category = Rendering)
	int32	MaterialID;
	//! �}�e���A���^�C���C���f�b�N�X
	UPROPERTY(EditDefaultsOnly, Category = Rendering)
	int32	TileID[ENaMapBlockFace::ENUM_MAX];

	//! ��
	UPROPERTY(EditDefaultsOnly, Category = Attributes)
	TEnumAsByte<ENaBlockPhase::Type>	Phase;
	//! ���߃u���b�N����
	UPROPERTY(EditDefaultsOnly, Category = Attributes)
	bool	IsOpaque;
	//! �o����Փx
	UPROPERTY(EditDefaultsOnly, Category = Attributes)
	bool	ClimbDiff;

	FNaBlockDataAsset()
	: UID( 0 )
	, IsRender( true )
	{
	}
};
