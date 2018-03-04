// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Assets/NaDataAsset.h"

#include "NaEventConst.h"

#include "NaEventAsset.generated.h"

USTRUCT()
struct NANRPG_API FNaEventCommand : public FTableRowBase
{
	GENERATED_BODY()

public:
	//! 実行コード
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	ENaEventCode	Code;

	//! 引数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Arguments")
	FString			Arg0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Arguments")
	FString			Arg1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Arguments")
	FString			Arg2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Arguments")
	FString			Arg3;
};

/**
 * イベントシート
 */
USTRUCT(BlueprintType)
struct NANRPG_API FNaEventSheet
{
	GENERATED_BODY()
	
public:
	//! 発動トリガー
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trigger")
	ENaEventTrigger	Trigger;

	//! アクティブ条件
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Condition")


	//! イベントレコード
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Record")
	UDataTable*		Record;

	//! コマンドリスト生成
	void	GetCommandList( TArray<FNaEventCommand*>& outArray ) const
	{
		Record->GetAllRows( FString(), outArray );
	}
};

/**
 * イベント定義アセット
 */
UCLASS()
class NANRPG_API UNaEventAsset : public UNaDataAsset
{
	GENERATED_BODY()
	
public:

public:
	//! 表示名
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FText	DisplayName;

	//! イベントシート（後にあるもの優先）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sheets")
	TArray<FNaEventSheet>	Sheets;
};
