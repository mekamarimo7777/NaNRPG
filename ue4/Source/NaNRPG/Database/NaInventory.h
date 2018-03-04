// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item/NaItem.h"

#include "NaInventory.generated.h"

class UNaEntityItem;

/**
 * �C���x���g���iNaEntity�̏W�����Ǘ��j
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

	//! �������X�V
	void	UpdateMoney();
	//! 
	int32	GetMoney() const	{ return m_Money; }

	//! �d�ʍX�V
	void	UpdateWeight();
	
public:
	UPROPERTY()
	FString	m_Name;

	UPROPERTY()
	int32	m_Capacity;

	//! �A�C�e��
	UPROPERTY()
	TArray<UNaItem*>	m_pItems;
	//! ������
	UPROPERTY()
	int32				m_Money;
	//! ���d��
	UPROPERTY()
	int32				m_Weight;
};
