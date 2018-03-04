// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "NaMapPartsAsset.h"

#include "World/NaWorldDefinition.h"

#include "NaMapAsset.generated.h"

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

	//@obsolete
	UPROPERTY()
	TAssetPtr<UNaMapPartsAsset>	PartsAsset;
	//@obsolete
	UPROPERTY()
	UNaMapPartsAsset*	Parts;	// ����
	//@obsolete
	int32	TestIdx;
};

// �}�b�v�����ǉ������v���Z�X
USTRUCT()
struct FNaMapProcess
{
	GENERATED_BODY()

	UPROPERTY()
	int32			Type;		// �v���Z�X���
//	UPROPERTY()
//	TArray<FNaMapProcessCond>	Conditions;		// ������� //
	UPROPERTY()
	TArray<int32>	Params;		// �p�����[�^ //
};

struct FNaMapGen;
struct FNaMapNode;

/**
 * �}�b�v�f�[�^�A�Z�b�g
 */
UCLASS()
class NANRPG_API UNaMapAsset : public UObject
{
	GENERATED_BODY()
	
public:
	// �ȈՃ����_���}�b�v���� //
	void	CreateSimpleRandomMap( FIntVector size );
	//
	void	DivideNode( FNaMapGen* gen );
	//
	void	CreateRoom( FNaMapGen* gen );
	//
	void	CreateCorridor( FNaMapGen* gen );
	//
	void	ConnectRoom( FNaMapNode* src, FNaMapNode* dst );

	//! 
	void	ResizeMap( const FIntVector& size );
	//! 
	void	SetBlock( const FIntVector& pos, const FNaWorldBlockWork& work );

	//! 
	TArray<FNaChunkData>&	GetChunks()	{ return m_Chunks; }

public:
	//! ID
	UPROPERTY()
	int32		m_ID;
	//! �}�b�v�T�C�Y�i�`�����N�P�ʁj
	UPROPERTY()
	FIntVector	m_MapSize;

	//! �ŗL�`�����N
	UPROPERTY()
	TArray<FNaChunkData>		m_Chunks;
	//! �\�������X�g
	UPROPERTY()
	TArray<FNaMapPartsEntry>	m_Parts;

	//! �G���e�B�e�B���X�g
	UPROPERTY(EditDefaultsOnly, Category = "Entity")
	TArray<FNaEntityEntry>		m_Entities;
	//! ���ꏈ��
	UPROPERTY()
	TArray<FNaMapProcess>		m_Process;

	//! �ꎞ���

};
