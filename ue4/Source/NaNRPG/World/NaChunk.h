// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "World/NaWorldDefinition.h"

#include "NaChunk.generated.h"

class UNaRegion;
class UNaEntity;

#define	CELL_OFS(x, y, z)	(x + (y << UNaChunk::SHIFT_Y) + (z << UNaChunk::SHIFT_Z))

/**
 * �`�����N���
 */
UCLASS()
class NANRPG_API UNaChunk : public UObject
{
	GENERATED_BODY()
	
public:
	static const int	DIM_X = 16;
	static const int	DIM_Y = 16;
	static const int	DIM_Z = 16;
	static const int	SHIFT_Y = 4;
	static const int	SHIFT_Z = 8;
	static const int	ELEMENTS = DIM_X * DIM_Y * DIM_Z;

public:
	// ������
	void		Initialize( UNaRegion* region, FIntVector cpos );
	// �`�����N�I�[�v��
	void		Open();
	// �`�����N�N���[�Y
	void		Close();

	//! 
	void		SetPosition( FIntVector pos )	{m_ChunkPos = pos;}
	// �`�����N���W�擾�i���[�W������ԁj
	FIntVector	GetPosition() const				{return m_ChunkPos;}
	// ���[���h��Ԃ̃`�����N���W�擾
	FIntVector	GetPositionInWorld() const;

	//
	FORCEINLINE UNaRegion*	GetRegion() const	{return m_pRegion;}
	//
	FORCEINLINE int16	GetBlockID( int32 x, int32 y, int32 z ) const
	{
		return m_BlockID[ CELL_OFS(x, y, z) ];
	}
	//
	FORCEINLINE FNaWorldBlockMeta	GetMetaData( int32 x, int32 y, int32 z )
	{
		return m_MetaData[ CELL_OFS(x, y, z) ];
	}

	//! �u���b�N�f�[�^�ݒ�
	void	SetBlock( const FIntVector& pos, const FNaWorldBlockWork& inVal );
	void	SetBlock( int32 x, int32 y, int32 z, const FNaWorldBlockWork& inVal );
	void	SetBlock( int32 idx, const FNaWorldBlockWork& inVal );
	//! �u���b�N�f�[�^�擾
	bool	GetBlock( const FIntVector& pos, FNaWorldBlockWork& outVal );
	bool	GetBlock( int32 x, int32 y, int32 z, FNaWorldBlockWork& outVal );
	bool	GetBlock( int32 idx, FNaWorldBlockWork& outVal );

	//! �u���b�N�f�[�^�ꊇ�R�s�[
	void	StoreBlocks( const uint8* id, const uint8* param, const uint32* meta );

	// �L���u���b�N�C���f�b�N�X��
	void	EnumValidCellIndices( TArray<int16>& outVal );

	// �G���e�B�e�B�o�^ //
	void	AttachEntity( UNaEntity* entity );
	// �G���e�B�e�B���� //
	void	DetachEntity( UNaEntity* entity );

	// �o�C�g�񂩂�f�[�^���� //
	void	ArrayToChunk( const TArray<uint8>& buff );
	// �`�����N���o�C�g��ɕϊ� //
	void	ChunkToArray( TArray<uint8>& outBuff );

	//! �V���A���C�Y
	virtual void	Serialize( FArchive& ar ) override;

	// 
	void	CreateTestChunk();
	//@obsolete
	//void	RecalcVisibleFaces();
	//
	void	RecalcRenderCells();
	//
	void	RecalcNeighborCells( int32 dir );
	//! 
	bool	CheckVisibleFace( int32 index, int32 dir );

	//
	void	ConnectNeighborChunk( int32 dir, UNaChunk* neighbor );
	//
	void	DisconnectNeighborChunk( int32 dir );
	//! 
	int32	GetReverseDir( int32 dir ) const;
	//
	FORCEINLINE UNaChunk*	GetNeighborChunk( int32 dir ) const	{return m_pNeighborChunk[dir];}

	//
	int32	GetRevision() const	{return m_Revision;}

	//! 
	const TArray<int16>&	GetRenderCells() const	{ return m_RenderCells; }

	//! 
	void	SetInvalidate( bool value );
	//! 
	bool	IsInvalidate() const		{ return m_bInvalidate; }

public:
	
protected:
	//
	UNaRegion*	m_pRegion;
	// �`�����N���W�i���[�W������ԁj
	FIntVector	m_ChunkPos;
	//
	bool		m_bOpen;
	int32		m_Revision;		// �A�N�^���̍X�V�`�F�b�N�p�i���j //
	int32		m_PrevCeilZ;	// 

	// �u���b�N���i�\���̂���Ȃ��͈̂��k������̂��߁j //
	// �u���b�NID
	uint8				m_BlockID[ELEMENTS];
	// �u���b�N�p�����[�^
	uint8				m_BlockParam[ELEMENTS];
	// �ėp�f�[�^
	FNaWorldBlockMeta	m_MetaData[ELEMENTS];
	// ���邳
	uint8				m_Blightness[ELEMENTS / 2];

	// �u���b�N�G���e�B�e�B //
//	UPROPERTY(Transient)
//	TArray<UNaBlockEntity*>			m_CellEntities;	// 
//	TArray<int16, UNaBlockEntity*>	m_CellEntityMap;

	// �`�����N�Ǘ��̃G���e�B�e�B���� //
	UPROPERTY(Transient)
	TArray<UNaEntity*>	m_Entities;	// �`�����N�X�e�[�W�̃G���e�B�e�B //

	//! �`��u���b�N 
	TArray<int16>	m_RenderCells;
	//! �`��ʁi�ꎞ�f�[�^�j
	int32			m_VisibleFace[ELEMENTS];

	//! �X�V�t���O
	bool	m_bInvalidate;

	// �אڃ`�����N //
	UPROPERTY(Transient)
	UNaChunk*	m_pNeighborChunk[6];	// �_�~�[���̃T�C�Y8�ɂ���H
};
