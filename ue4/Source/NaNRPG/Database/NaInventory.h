// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item/NaItem.h"

#include "NaInventory.generated.h"

class UNaEntityItem;

/**
 * インベントリ（NaEntityの集合を管理）
 */
UCLASS()
class NANRPG_API UNaInventory : public UObject
{
	GENERATED_BODY()

public:
	//!
	bool	AddItem( UNaItem* item );
	//!
	void	RemoveItem( UNaItem* item );

	//! 所持金更新
	void	UpdateMoney();
	//! 
	int32	GetMoney() const	{ return m_Money; }

	//! 重量更新
	void	UpdateWeight();
	
public:
	UPROPERTY()
	FString	m_Name;

	UPROPERTY()
	int32	m_Capacity;

	//! アイテム
	UPROPERTY()
	TArray<UNaItem*>	m_pItems;
	//! 所持金
	UPROPERTY()
	int32				m_Money;
	//! 総重量
	UPROPERTY()
	int32				m_Weight;
};
