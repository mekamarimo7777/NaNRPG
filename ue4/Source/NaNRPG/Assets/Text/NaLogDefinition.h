// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "Misc/NaStructs.h"

#include "NaLogDefinition.generated.h"

USTRUCT()
struct FNaLogMessage
{
	GENERATED_BODY()

	//! ID
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FName			Name;
	//! 表示テキスト
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FText			Text;
	//! カラー
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FLinearColor	Color;

	FNaLogMessage()
	: Color( FLinearColor::White )
	{
	}
};

/**
 * ログ表示定義
 */
UCLASS()
class NANRPG_API UNaLogDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//! ログテキスト検索
	const FNaLogMessage*	FindMessage( FName name ) const;
	//! 
	FText	GetActionText( ENaActionTrigger::Type action )	{ return ActionTexts[action]; }

public:
	//! メッセージ
	UPROPERTY(EditDefaultsOnly, Category = "General")
	TArray<FNaLogMessage>	Messages;
	//! アクションテキスト
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FText					ActionTexts[ENaActionTrigger::ENUM_MAX];
};
