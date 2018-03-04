// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc/NaEnums.h"

#include "Assets/Item/NaItemDataTable.h"
#include "Assets/Item/NaMatterDataTable.h"

#include "NaItem.generated.h"

class UNaEntity;

/**
 * �A�C�e���C���X�^���X
 */
UCLASS()
class NANRPG_API UNaItem : public UObject
{
	GENERATED_BODY()
	
public:
	//! �A�Z�b�g�����{�f�[�^�\�z
	void	CreateFromAsset( const FNaItemDataAsset& asset );

	//! �f�ޓK�p
	void	ApplyMatter( FName matterName );

	//! �G���e�B�e�B�𐶐�
	UNaEntityCharacter*	GenerateEntity();

	//! 
	void		SetOwner( UNaEntity* entity )	{ m_Entity = entity; }
	//! 
	UNaEntity*	GetOwner() const				{ return m_Entity; }

	//! 
	UFUNCTION( BlueprintCallable, Category = "Item" )
	int32	GetAssetID() const		{ return m_AssetID; }
	//! 
	UFUNCTION( BlueprintCallable, Category = "Item" )
	int32	GetEntityParam() const	{ return m_EntityParam; }
	//! 
	UFUNCTION( BlueprintCallable, Category = "Item" )
	int32	GetIconID() const		{ return m_IconID; }
	//! 
	UFUNCTION( BlueprintCallable, Category = "Item" )
	FVector	GetHSV() const			{ return FVector( m_IconHue / 360.0f, m_IconSaturation / 128.0f, 0.0f ); }
	//! 
	FString	GetDisplayName() const	{ return m_DisplayName; }
	//! 
	const TArray<FNaItemProperty>&	GetProperties() const	{ return m_Properties; }

	//! �\�A�N�V��������
	bool	HasAction( ENaActionTrigger::Type action );
	//! �\�A�N�V��������i�t���O�j
	bool	HasActionFlag( int32 actionFlag );

	//! ���ʔ���
	bool	HasEffect( ENaActionEffect::Type effect );

	//! ���ʗ�
	bool	EnumProperties( ENaActionEffect::Type effect, TArray<FNaItemProperty*>& outVal );

	//! �X�^�b�N���ǉ��i�߂�l�͎��ۂɑ��������l�j
	int32	AddStack( int32 value );
	//! �X�^�b�N�\����
	bool	IsStackable( UNaItem* item );
	//! 
	UFUNCTION( BlueprintCallable, Category = "Item" )
	int32	GetMaxStackCount() const	{ return m_MaxStackCount; }
	//! 
	UFUNCTION( BlueprintCallable, Category = "Item" )
	int32	GetStackCount() const		{ return m_StackCount; }

	//! 
	UFUNCTION( BlueprintCallable, Category = "Item" )
	int32	GetWeight() const			{ return 0; }

	//! �����\���ʔ���
	bool	CanEquipment( ENaItemEquipPart::Type part ) const	{ return (m_EquipFlag & (1 << int32(part))) != 0; }
	//! 
	void	SetEquip( bool isEquip )	{ m_IsEquip = isEquip; }
	//! 
	bool	IsEquip() const				{ return m_IsEquip; }

	//! �V���A���C�Y
	virtual	void	Serialize( FArchive& ar ) override;

public:

protected:
	//! �I�[�i�[�G���e�B�e�B
	UPROPERTY(Transient)
	UNaEntity*	m_Entity;

	//! ���A�Z�b�gID
	int32		m_AssetID;

	//! �\����
	FString		m_DisplayName;

	//! �G���e�B�e�B�p
	FName		m_EntityName;
	int32		m_EntityParam;
	int16		m_IconID;
	int16		m_IconHue;
	int16		m_IconSaturation;

	//! 
	int16		m_MaxStackCount;
	int16		m_StackCount;

	//! �����\����
	int32		m_EquipFlag;
	bool		m_IsEquip;

	//! �f�ޖ�
	FName		m_MatterName;

	//! ���ʃ��X�g
	TArray<FNaItemProperty>	m_Properties;



/*	FString		m_Name;				// ���O
	FString		m_UnknownName;		// �s�m�薼

	int32		m_Price;			// ��{���i
	int32		m_Rarity;			// ���A���e�B
	int32		m_Weight;			// �d��
	int32		m_Identification;	// �Ӓ�x
	int32		m_Reinforce;		// �����l

	TArray<FNaItemProperty>	m_Properties;*/
};
