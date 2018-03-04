// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "NaEntityManipulator.generated.h"

class UNaEntityCharacter;

/**
 * エンティティ操作基底
 */
UCLASS(abstract)
class NANRPG_API UNaEntityManipulator : public UObject
{
	GENERATED_BODY()
	
public:
	// 操作ターゲット設定
	void	BindTarget( UNaEntityCharacter* target );

	// 更新
	void	UpdateControl( float DeltaTime );

	// 行動終了
	bool	IsEndAction() const	{return true;}

protected:
	// 更新処理
	virtual void	OnControl( float DeltaTime );

	//! 
	UWorld*	GetWorldContext() const;

public:
	
protected:
	UPROPERTY()
	UNaEntityCharacter*	m_Target;
};
