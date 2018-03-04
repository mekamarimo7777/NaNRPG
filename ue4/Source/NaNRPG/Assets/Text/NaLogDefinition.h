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
	//! �\���e�L�X�g
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FText			Text;
	//! �J���[
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FLinearColor	Color;

	FNaLogMessage()
	: Color( FLinearColor::White )
	{
	}
};

/**
 * ���O�\����`
 */
UCLASS()
class NANRPG_API UNaLogDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//! ���O�e�L�X�g����
	const FNaLogMessage*	FindMessage( FName name ) const;
	//! 
	FText	GetActionText( ENaActionTrigger::Type action )	{ return ActionTexts[action]; }

public:
	//! ���b�Z�[�W
	UPROPERTY(EditDefaultsOnly, Category = "General")
	TArray<FNaLogMessage>	Messages;
	//! �A�N�V�����e�L�X�g
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FText					ActionTexts[ENaActionTrigger::ENUM_MAX];
};
