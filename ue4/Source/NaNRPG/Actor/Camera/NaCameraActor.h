// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraActor.h"

#include "NaCameraActor.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class NANRPG_API ANaCameraActor : public ACameraActor
{
	GENERATED_BODY()
	
public:
	//! コンストラクタ
	ANaCameraActor( const FObjectInitializer& ObjectInitializer );

	//! 開始
	virtual void BeginPlay() override;

	//! 更新処理
	virtual void Update( float DeltaSeconds );

public:
	//! バインド先設定
	void	AttachTarget( AActor* target );
	//! アングル設定
	void	SetAngle( float angle );

protected:
	//! カメラ角度設定
	void	ApplyCameraAngle();

public:
	//! カメラピッチ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraActor)
	float	OrbitalPitch;
	//! カメラ距離
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraActor)
	float	OrbitalRange;

protected:
	//! 注視ターゲット
	UPROPERTY()
	AActor*	m_pTarget;

	//! 
	FVector	m_CurrentPos;
	//! カレント角度
	float	m_CurrentAngle;
	//! リクエスト角度
	float	m_RequestAngle;
};
