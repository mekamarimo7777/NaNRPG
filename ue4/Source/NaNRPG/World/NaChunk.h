// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "World/NaWorldDefinition.h"

#include "NaChunk.generated.h"

class UNaRegion;
class UNaEntity;

#define	CELL_OFS(x, y, z)	(x + (y << UNaChunk::SHIFT_Y) + (z << UNaChunk::SHIFT_Z))

/**
 * チャンク情報
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
	// 初期化
	void		Initialize( UNaRegion* region, FIntVector cpos );
	// チャンクオープン
	void		Open();
	// チャンククローズ
	void		Close();

	//! 
	void		SetPosition( FIntVector pos )	{m_ChunkPos = pos;}
	// チャンク座標取得（リージョン空間）
	FIntVector	GetPosition() const				{return m_ChunkPos;}
	// ワールド空間のチャンク座標取得
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

	//! ブロックデータ設定
	void	SetBlock( const FIntVector& pos, const FNaWorldBlockWork& inVal );
	void	SetBlock( int32 x, int32 y, int32 z, const FNaWorldBlockWork& inVal );
	void	SetBlock( int32 idx, const FNaWorldBlockWork& inVal );
	//! ブロックデータ取得
	bool	GetBlock( const FIntVector& pos, FNaWorldBlockWork& outVal );
	bool	GetBlock( int32 x, int32 y, int32 z, FNaWorldBlockWork& outVal );
	bool	GetBlock( int32 idx, FNaWorldBlockWork& outVal );

	//! ブロックデータ一括コピー
	void	StoreBlocks( const uint8* id, const uint8* param, const uint32* meta );

	// 有効ブロックインデックス列挙
	void	EnumValidCellIndices( TArray<int16>& outVal );

	// エンティティ登録 //
	void	AttachEntity( UNaEntity* entity );
	// エンティティ除去 //
	void	DetachEntity( UNaEntity* entity );

	// バイト列からデータ生成 //
	void	ArrayToChunk( const TArray<uint8>& buff );
	// チャンクをバイト列に変換 //
	void	ChunkToArray( TArray<uint8>& outBuff );

	//! シリアライズ
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
	// チャンク座標（リージョン空間）
	FIntVector	m_ChunkPos;
	//
	bool		m_bOpen;
	int32		m_Revision;		// アクタ側の更新チェック用（仮） //
	int32		m_PrevCeilZ;	// 

	// ブロック情報（構造体じゃないのは圧縮率向上のため） //
	// ブロックID
	uint8				m_BlockID[ELEMENTS];
	// ブロックパラメータ
	uint8				m_BlockParam[ELEMENTS];
	// 汎用データ
	FNaWorldBlockMeta	m_MetaData[ELEMENTS];
	// 明るさ
	uint8				m_Blightness[ELEMENTS / 2];

	// ブロックエンティティ //
//	UPROPERTY(Transient)
//	TArray<UNaBlockEntity*>			m_CellEntities;	// 
//	TArray<int16, UNaBlockEntity*>	m_CellEntityMap;

	// チャンク管理のエンティティ実体 //
	UPROPERTY(Transient)
	TArray<UNaEntity*>	m_Entities;	// チャンクステージのエンティティ //

	//! 描画ブロック 
	TArray<int16>	m_RenderCells;
	//! 描画面（一時データ）
	int32			m_VisibleFace[ELEMENTS];

	//! 更新フラグ
	bool	m_bInvalidate;

	// 隣接チャンク //
	UPROPERTY(Transient)
	UNaChunk*	m_pNeighborChunk[6];	// ダミーつきのサイズ8にする？
};
