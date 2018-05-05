// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaInventory.h"




//!
bool UNaInventory::AddItem( UNaItem* item )
{
	//! スタック追加
	for ( auto& it : m_pItems ){
		if ( it->IsStackable( item ) ){
			int32	tmp;

			tmp	= it->AddStack( item->GetStackCount() );
			item->AddStack( -tmp );

			if ( item->GetStackCount() <= 0 ){
				return true;
			}
		}
	}
	
	m_pItems.Add( item );
	m_pItems.StableSort( []( const UNaItem& left, const UNaItem& right )
	{
		return left.GetAssetID() < right.GetAssetID();
	});

	if ( item->HasEffect( ENaActionEffect::Currency ) ){
		UpdateMoney();
	}

	return true;
}

//!
void UNaInventory::RemoveItem( UNaItem* item )
{
	m_pItems.Remove( item );

	if ( item->HasEffect( ENaActionEffect::Currency ) ){
		UpdateMoney();
	}
}

//! 所持金更新
void UNaInventory::UpdateMoney()
{
	TArray<FNaItemProperty*>	props;

	m_Money	= 0;

	for ( auto& it : m_pItems ){
		props.Reset();

		if ( it->EnumProperties( ENaActionEffect::Currency, props ) ){
			for ( auto& it2 : props ){
				m_Money	+= it2->Params[0] * it->GetStackCount();
			}
		}
	}
}

//! 重量更新
void UNaInventory::UpdateWeight()
{
}
