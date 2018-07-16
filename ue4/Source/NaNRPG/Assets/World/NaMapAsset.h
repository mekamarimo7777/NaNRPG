// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Assets/NaDataAsset.h"

#include "World/NaWorldDefinition.h"

#include "NaMapPartsAsset.h"

#include "NaMapAsset.generated.h"

//! �����_���}�b�v�����������
UENUM()
enum class ENaRandomMapRoom
{
	//! ��{�������[�`��
	Normal,
	//! �~��z�u
	Circle,

	ENUM_MAX	UMETA(Hidden)
};

//! �p�[�c�G���g�����
USTRUCT()
struct FNaMapPartsEntry
{
	GENERATED_BODY()

	//! �p�[�c�A�Z�b�gID
	UPROPERTY()
	FName		ID;
	//! �z�u���W
	UPROPERTY()
	FIntVector	Position;
	//! �z�u�T�C�Y
	UPROPERTY()
	FIntVector	Size;
	//! ����
	UPROPERTY()
	int32		Direction;
};

//! �}�b�v�ŗL�`�����N���
USTRUCT()
struct FNaChunkData
{
	GENERATED_BODY()

	static const int	DIM_X = 16;
	static const int	DIM_Y = 16;
	static const int	DIM_Z = 16;
	static const int	SHIFT_Y = 4;
	static const int	SHIFT_Z = 8;
	static const int	ELEMENTS = DIM_X * DIM_Y * DIM_Z;

	//! �`�����N���W
	UPROPERTY()
	FIntVector	Position;
	// �u���b�NID
	UPROPERTY()
	uint8		ID[ELEMENTS];
	// �u���b�N�p�����[�^
	UPROPERTY()
	uint8		Param[ELEMENTS];
	// �ėp�f�[�^
	UPROPERTY()
	uint32		Meta[ELEMENTS];
};

// �����_���}�b�v�p�p�[�c
USTRUCT()
struct FNaRandomMapParts
{
	GENERATED_BODY()

	//! �p�[�c�A�Z�b�gID
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FName		ID;
	//! �K�p�Ώہi����or�ʘH�j
	UPROPERTY(EditDefaultsOnly, Category = "General")
	bool		IsRoom;
	//! �g�p����i-1�Ŗ������j
	UPROPERTY(EditDefaultsOnly, Category = "General")
	int32		Limit;
};

// �����_���}�b�v���������p�����[�^
USTRUCT()
struct FNaRandomMapRoomParam
{
	GENERATED_BODY()

	//! ���
	UPROPERTY(EditDefaultsOnly, Category = "General")
	ENaRandomMapRoom	Type;
	//! �����͈͕�
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FIntVector			Range;

	//! ���z�͈�
	UPROPERTY(EditDefaultsOnly, Category = "Normal")
	float		Radius;
	//! �����_������������
	UPROPERTY(EditDefaultsOnly, Category = "Normal")
	int32		RoomGen;
	//! ���U�J�E���^

	//! �ŏ����������T�C�Y
	UPROPERTY(EditDefaultsOnly, Category = "Room")
	FIntPoint	MinRoomSize;
	//! �ő吶�������T�C�Y
	UPROPERTY(EditDefaultsOnly, Category = "Room")
	FIntPoint	MaxRoomSize;
	//! ���C�������T�C�Y臒l
	UPROPERTY(EditDefaultsOnly, Category = "Room")
	FIntPoint	MainRoomSize;
};

// �����_���}�b�v�����p�����[�^
USTRUCT()
struct FNaRandomMapParam
{
	GENERATED_BODY()

	//! ��������
	UPROPERTY(EditDefaultsOnly, Category = "General")
	TArray<FNaRandomMapRoomParam>	RoomProcess;
};

/**
 * �}�b�v�f�[�^�A�Z�b�g
 */
UCLASS()
class NANRPG_API UNaMapAsset : public UObject
{
	GENERATED_BODY()
	
public:
/*	// �ȈՃ����_���}�b�v���� //
	void	CreateSimpleRandomMap( FIntVector size );
	//
	void	DivideNode( FNaMapGen* gen );
	//
	void	CreateRoom( FNaMapGen* gen );
	//
	void	CreateCorridor( FNaMapGen* gen );
	//
	void	ConnectRoom( FNaMapNode* src, FNaMapNode* dst );
	*/
	//! 
	void	ResizeMap( const FIntVector& size );
	//! 
	void	SetBlock( const FIntVector& pos, const FNaWorldBlockWork& work );

	//! 
	TArray<FNaChunkData>&	GetChunks()	{ return Chunks; }

	//! �����_���}�b�v�p�����[�^�擾
	const FNaRandomMapParam*	GetRandomMapParam() const	{ return &RandomMap; }

protected:
	

public:
	//! �}�b�v�T�C�Y�i�`�����N�P�ʁj
	UPROPERTY()
	FIntVector	MapSize;

	//! �����_�������̎g�p
	UPROPERTY(EditDefaultsOnly, Category = "Generation")
	bool						UseRandomMap;
	//! �����_�������ݒ�
	UPROPERTY(EditDefaultsOnly, Category = "Generation")
	FNaRandomMapParam			RandomMap;
	//! �����_�������g�p�����p�[�c
	UPROPERTY(EditDefaultsOnly, Category = "Generation")
	TArray<FNaRandomMapParts>	RandomParts;

	//! �Œ�`�����N
	UPROPERTY()
	TArray<FNaChunkData>		Chunks;
	//! �Œ�z�u�\����
	UPROPERTY()
	TArray<FNaMapPartsEntry>	Parts;

	//! �G���e�B�e�B���X�g
	UPROPERTY(EditDefaultsOnly, Category = "Entity")
	TArray<FNaEntityEntry>		Entities;

	//! �ꎞ���

};
