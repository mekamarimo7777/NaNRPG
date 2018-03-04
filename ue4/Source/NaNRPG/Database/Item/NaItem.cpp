// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaItem.h"

#include "Entity/INaEntityFactory.h"
#include "Entity/Character/NaEntityCharacter.h"

#include "Assets/NaAssetLibrary.h"

//! �A�Z�b�g�����{�f�[�^�\�z
void UNaItem::CreateFromAsset( const FNaItemDataAsset& asset )
{
	m_AssetID		= asset.UID;

	m_DisplayName	= asset.DisplayName.ToString();

	m_EntityName	= asset.EntityName;
	if ( m_EntityName.IsNone() ){
		m_EntityName	= FName( "Item" );
	}
	m_EntityParam		= asset.EntityParam;
	m_IconID			= asset.IconID;
	m_IconHue			= asset.IconHue;
	m_IconSaturation	= asset.IconSaturation;

	m_EquipFlag		= asset.EquipFlag;

	m_MaxStackCount	= asset.MaxStackCount;
	m_StackCount	= 0;

	m_Properties	= asset.Properties;
}

//! �f�ޓK�p
void UNaItem::ApplyMatter( FName matterName )
{
}

//! �G���e�B�e�B�𐶐�
UNaEntityCharacter* UNaItem::GenerateEntity()
{
	UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
	UNaEntityCharacter*	entity = nullptr;
	FNaEntityDataAsset*	asset;

	asset	= alib->FindEntityAsset( m_EntityName );
	if ( asset ){
		entity	= INaEntityFactory::NewEntity<UNaEntityCharacter>( asset->Type );
		entity->CreateFromAsset( *asset );
		entity->SetItemProperty( this );

		SetOwner( entity );
	}

	return entity;
}

//! �\�A�N�V��������
bool UNaItem::HasAction( ENaActionTrigger::Type action )
{
	for ( auto& it : m_Properties ){
		if ( it.Action & (1 << int32(action)) ){
			return true;
		}
	}
	return false;
}

//! �\�A�N�V��������i�t���O�j
bool UNaItem::HasActionFlag( int32 actionFlag )
{
	for ( auto& it : m_Properties ){
		if ( it.Action & actionFlag ){
			return true;
		}
	}
	return false;
}

//! ���ʔ���
bool UNaItem::HasEffect( ENaActionEffect::Type effect )
{
	for ( auto& it : m_Properties ){
		if ( it.Effect == effect ){
			return true;
		}
	}
	return false;
}

//! ���ʎ擾
bool UNaItem::EnumProperties( ENaActionEffect::Type effect, TArray<FNaItemProperty*>& outVal )
{
	for ( auto& it : m_Properties ){
		if ( it.Effect == effect ){
			outVal.Add( &it );
		}
	}
	return outVal.Num() > 0;
}

//! �X�^�b�N���ǉ��i�߂�l�͎��ۂɑ��������l�j
int32 UNaItem::AddStack( int32 value )
{
	int32	prevValue = m_StackCount;

	m_StackCount	= FMath::Clamp( m_StackCount + value, 0, (int32)m_MaxStackCount );

	return m_StackCount - prevValue;
}

//! �X�^�b�N�\����
bool UNaItem::IsStackable( UNaItem* item )
{
	if ( m_MaxStackCount <= 1 ){
		return false;
	}

	//@ ������
	if ( m_AssetID != item->m_AssetID ){
		return false;
	}

	return true;
}

//! �V���A���C�Y
void UNaItem::Serialize( FArchive& ar )
{
	ar << m_AssetID;

	ar << m_DisplayName;

	SerializeFName( ar, m_EntityName );
	ar << m_EntityParam;
	ar << m_IconID;
	ar << m_IconHue;
	ar << m_IconSaturation;

	ar << m_MaxStackCount;
	ar << m_StackCount;

	ar << m_EquipFlag;
	ar << m_IsEquip;

	SerializeFName( ar, m_MatterName );

	ar << m_Properties;
}
