// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "ProceduralMeshComponent.h"

#include "Actor/Camera/NaCameraActor.h"

#include "NaMapChunkActor.h"

#include "World/NaWorld.h"

#include "NaWorldActor.generated.h"

class ANaActorBase;

// ワールドアクター
// 役割：マップデータ管理、各チャンクアクター、オブジェクトアクターの管理
UCLASS()
class NANRPG_API ANaWorldActor : public AActor
{
	GENERATED_BODY()

public:
	enum EState
	{
		Main,
	};
	
public:
	// 
	ANaWorldActor();

	// 
	virtual void BeginPlay() override;
	
	// 
	virtual void Tick( float DeltaSeconds ) override;

public:
	// @obsolete マップ読み込み //
	void	LoadMap( int32 mapID );

	// ワールド設定 //
	void	AssignWorld( UNaWorld* world );

	// //
	void	CloseWorld();

	//
	void	ChangeState(EState state, int32 param = 0, bool immediate = false);

	//! 
	void	SetRenderSize( FIntVector size, FIntVector margin = FIntVector::ZeroValue );
	//! 
	void	UpdateChunkActor();
	//! 
	ANaMapChunkActor*	GetChunkActor( const FIntVector& chunkPos );

	//
	void	UpdateMap( const FIntVector& pos );

	//
	UMaterialInstanceDynamic*	FindBlockMaterial(int32 id);

	//! カメラセット
	void	SetCamera(ANaCameraActor* camera)	{ m_Camera = camera; }

protected:
	//
	void	ProcMain(float DeltaTime);
	
public:
	//! ポストプロセスマテリアル
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PostProcess")
	UMaterialInstance*	PostProcessMaterial;

protected:
	//! ゲームワールド
	UPROPERTY()
	UNaWorld*		m_World;

	//! チャンクアクター
//	TMap<FIntVector, ANaMapChunkActor*>	m_ChunkActors;
	//! 管理チャンクアクター
	UPROPERTY()
	TArray<ANaMapChunkActor*>	m_ChunkActors;

	//! 描画チャンク距離
	UPROPERTY()
	FIntVector		m_RenderSize;
	//! 描画チャンク距離マージン
	UPROPERTY()
	FIntVector		m_RenderSizeMargin;

	//! 視界遮蔽用キャプチャ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneCaptureComponentCube*	m_CaptureCube;
	//! ポストプロセス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UPostProcessComponent*		m_PostProcess;

	//! カレント座標
	FIntVector		m_CurrentPos;
	//! カレントチャンク
	FIntVector		m_CurrentChunkPos;

	//! プレイヤーアクター
	UPROPERTY()
	ANaActorBase*	m_Player;

	// ワールドアクター実体（プレイヤー、敵、その他動くもの） //
	UPROPERTY()
	TArray<ANaActorBase*>	m_WorldActors;
	// 行動中アクター //
	UPROPERTY()
	ANaActorBase*			m_pCurrentActor;
	// 行動待ちキュー //
	UPROPERTY()
	TArray<ANaActorBase*>	m_ActionActors;

	//! カメラ
	UPROPERTY()
	ANaCameraActor*			m_Camera;

	// ライト //
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Light")
	UDirectionalLightComponent*	m_pDirLight;

	// マテリアル //
	UPROPERTY()
	TMap<int32, UMaterialInstanceDynamic*>	m_MIDMap;
	//! ポストプロセスマテリアル
	UPROPERTY()
	UMaterialInstanceDynamic*				m_MIDPost;

	//! ステート
	EState	m_State;
	int32	m_StateParam;
	int32	m_StateStep;
};
