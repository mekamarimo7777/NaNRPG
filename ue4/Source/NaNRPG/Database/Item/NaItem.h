// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc/NaEnums.h"

#include "Assets/Item/NaItemDataTable.h"
#include "Assets/Item/NaMatterDataTable.h"

#include "NaItem.generated.h"

class UNaEntity;

/**
 * アイテムインスタンス
 */
UCLASS()
class NANRPG_API UNaItem : public UObject
{
	GENERATED_BODY()
	
public:
	//! アセットから基本データ構築
	void	CreateFromAsset( const FNaItemDataAsset& asset );

	//! 素材適用
	void	ApplyMatter( FName matterName );

	//! エンティティを生成
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

	//! 可能アクション判定
	bool	HasAction( ENaActionTrigger::Type action );
	//! 可能アクション判定（フラグ）
	bool	HasActionFlag( int32 actionFlag );

	//! 効果判定
	bool	HasEffect( ENaActionEffect::Type effect );

	//! 効果列挙
	bool	EnumProperties( ENaActionEffect::Type effect, TArray<FNaItemProperty*>& outVal );

	//! スタック数追加（戻り値は実際に増減した値）
	int32	AddStack( int32 value );
	//! スタック可能判定
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

	//! 装備可能部位判定
	bool	CanEquipment( ENaItemEquipPart::Type part ) const	{ return (m_EquipFlag & (1 << int32(part))) != 0; }
	//! 
	void	SetEquip( bool isEquip )	{ m_IsEquip = isEquip; }
	//! 
	bool	IsEquip() const				{ return m_IsEquip; }

	//! シリアライズ
	virtual	void	Serialize( FArchive& ar ) override;

public:

protected:
	//! オーナーエンティティ
	UPROPERTY(Transient)
	UNaEntity*	m_Entity;

	//! 元アセットID
	int32		m_AssetID;

	//! 表示名
	FString		m_DisplayName;

	//! エンティティ用
	FName		m_EntityName;
	int32		m_EntityParam;
	int16		m_IconID;
	int16		m_IconHue;
	int16		m_IconSaturation;

	//! 
	int16		m_MaxStackCount;
	int16		m_StackCount;

	//! 装備可能部位
	int32		m_EquipFlag;
	bool		m_IsEquip;

	//! 素材名
	FName		m_MatterName;

	//! 効果リスト
	TArray<FNaItemProperty>	m_Properties;



/*	FString		m_Name;				// 名前
	FString		m_UnknownName;		// 不確定名

	int32		m_Price;			// 基本価格
	int32		m_Rarity;			// レアリティ
	int32		m_Weight;			// 重さ
	int32		m_Identification;	// 鑑定度
	int32		m_Reinforce;		// 強化値

	TArray<FNaItemProperty>	m_Properties;*/
};
