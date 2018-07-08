// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "NaChunk.h"

#include "NaRegion.generated.h"

class UNaWorld;
class UNaMap;

/**
 * 
 */
UCLASS()
class NANRPG_API UNaRegion : public UObject
{
	GENERATED_BODY()
	
public:
	//!
	void	Initialize( UNaWorld* world, FIntVector rpos );

	//! リージョンオープン
	void	Open();
	//! リージョンクローズ
	void	Close();





	//
	bool	OpenRegion( const FString& path, FIntVector rpos );
	//
	bool	CreateRegion( const FString& path, FIntVector rpos );
	//
	void	CloseRegion();

	//
	UNaChunk*	GetChunk( FIntVector cpos );
	//
	UNaChunk*	CreateChunk( FIntVector cpos );
	// チャンククローズ（キャッシュに移動）
	void		CloseChunk( UNaChunk* chunk );
	void		CloseChunk( FIntVector cpos );

	//
	void	RecalcVisibleFaces();

	// シリアライズ //
	virtual void	Serialize( FArchive& ar ) override;

	//
	FORCEINLINE UNaWorld*	GetNaWorld() const			{return m_World;}
	//
	FORCEINLINE FIntVector	GetRegionPos() const		{return m_Location;}
	// 
	FORCEINLINE int32		GetActiveChunkCount() const	{return m_Chunks.Num();}

	//
	void	ConnectChunk( UNaChunk* chunk );
	//
	void	DisconnectChunk( UNaChunk* chunk );
	
protected:
	//! ワールド
	UPROPERTY(Transient)
	UNaWorld*	m_World;

	//! ファイルパス
	FString		m_FilePath;
	//! リージョン座標
	FIntVector	m_Location;

	// ロード中のチャンク情報 //
	UPROPERTY(Transient)
	TArray<UNaChunk*>			m_Chunks;
	TMap<FIntVector, UNaChunk*>	m_ChunkMap;

	// チャンクキャッシュ（圧縮バイトデータ） //
	// アンロード対象になったチャンクは一旦こちらに格納され、リージョン削除時にファイルへ書き込まれる //
	TMap<FIntVector, TArray<uint8> >	m_ChunkCache;

	UPROPERTY(Transient)
	TArray<UNaMap*>				m_MapData;		// 参照マップ
};
