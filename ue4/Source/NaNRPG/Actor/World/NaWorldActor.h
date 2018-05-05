// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "ProceduralMeshComponent.h"

#include "Actor/Camera/NaCameraActor.h"

#include "NaMapChunkActor.h"

#include "World/NaWorld.h"

#include "Utility/Components/NaStateMachine.h"

#include "NaWorldActor.generated.h"

class ANaActorBase;

// ワールドアクター
// 役割：マップデータ管理、各チャンクアクター、オブジェクトアクターの管理
UCLASS()
class NANRPG_API ANaWorldActor : public AActor
{
	GENERATED_BODY()

public:
	//! ステート
	enum EState
	{
		Main,
	};
	
public:
	//! コンストラクタ
	ANaWorldActor( const FObjectInitializer& ObjectInitializer );

	//! 開始処理
	virtual void BeginPlay() override;
	//! 更新
	virtual void Tick( float DeltaSeconds ) override;

	//! ワールドオープン
	UNaWorld*	OpenWorld( FName id, FName assetID = FName() );
	//! ワー	ルドクローズ
	void		CloseWorld( FName id );
	//! アクティブワールド変更リクエスト
	void		SwitchWorld( FName id );

	//! カメラ設定
	void	BindCamera( ANaCameraActor* camera )	{ m_Camera = camera; }

	//! 表示チャンク範囲設定
	void	SetRenderSize( FIntVector size, FIntVector margin = FIntVector::ZeroValue );
	//! チャンクアクター取得
	ANaMapChunkActor*	GetChunkActor( const FIntVector& chunkPos );
	//! ブロックマテリアル取得
	UMaterialInstanceDynamic*	FindBlockMaterial(int32 id);

	//! エンティティアクター生成
	ANaActorBase*	SpawnEntityActor( UNaEntity* entity );

protected:
	//! メイン
	void	ProcMain( UNaStateMachine* sm, float DeltaTime );

	//! チャンクアクター更新
	void	UpdateChunkActor();
	
public:
	//! ポストプロセスマテリアル
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PostProcess")
	UMaterialInstance*	PostProcessMaterial;

protected:
	//! 状態管理
	UPROPERTY()
	UNaStateMachine*	m_SM;

	//! オープン中のワールド
	UPROPERTY()
	TArray<UNaWorld*>	m_Worlds;
	//! アクティブワールド（表示中のワールド）
	UPROPERTY()
	UNaWorld*			m_ActiveWorld;
	//! トランジション予約先ワールド
	UPROPERTY()
	UNaWorld*			m_NextWorld;

	//! 管理チャンクアクター
	UPROPERTY()
	TArray<ANaMapChunkActor*>	m_ChunkActors;
//	TMap<FIntVector, ANaMapChunkActor*>	m_ChunkActors;
	//! 描画チャンク距離
	UPROPERTY()
	FIntVector		m_RenderSize;
	//! 描画チャンク距離マージン
	UPROPERTY()
	FIntVector		m_RenderSizeMargin;

	//! プレイヤーアクター
	UPROPERTY()
	ANaActorBase*	m_Player;
	//! カレント座標
	FIntVector		m_CurrentPos;
	//! カレントチャンク
	FIntVector		m_CurrentChunkPos;

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
	//! ライト
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Light")
	UDirectionalLightComponent*	m_pDirLight;

	//! 視界遮蔽用キャプチャ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneCaptureComponentCube*	m_CaptureCube;
	//! ポストプロセス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UPostProcessComponent*		m_PostProcess;

	// マテリアル //
	UPROPERTY()
	TMap<int32, UMaterialInstanceDynamic*>	m_MIDMap;
	//! ポストプロセスマテリアル
	UPROPERTY()
	UMaterialInstanceDynamic*				m_MIDPost;
};
