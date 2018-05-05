// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaEntityCharacter.h"

#include "Actor/Entity/NaActorBase.h"

#include "Assets/NaAssetLibrary.h"

#include "Entity/Controller/NaAIManipulator.h"

#include "Database/Item/NaItem.h"

#include "Singleton/NaGameLogger.h"

#include "UI/Widgets/Common/NaPopupDamage.h"

//
void UNaEntityCharacter::OnAwake()
{
	m_TurnAction	= NewObject<UNaTurnActionComponent>();
	if ( m_TurnAction ){
		m_TurnAction->BeginTurnEvent().BindUFunction( this, "OnBeginTurn" );
		m_TurnAction->UpdateTurnEvent().BindUFunction( this, "OnUpdateTurn" );
	}

	if ( !m_Manipulator ){
		m_Manipulator	= NewObject<UNaAIManipulator>();
		m_Manipulator->BindTarget( this );
	}

	if ( !m_Inventory ){
		m_Inventory	= NewObject<UNaInventory>();
	}

}
//!
void UNaEntityCharacter::OnSpawn()
{
	m_Status.HP	= m_Status.HPMax	= 20;
	m_Status.SP	= m_Status.SPMax	= 15;
	m_Status.MP	= m_Status.MPMax	= 10;

	m_Status.BaseParam.Str	= 5;

}

//
void UNaEntityCharacter::OnEnter()
{
	UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
	UWorld*	const		world = m_pWorld->GetWorldContext();

	// アクター生成
	if ( alib ){
		const FNaActorDataAsset*	asset = alib->FindActorAsset( m_Profile.ActorName );

		if ( asset ){
			m_Actor	= world->SpawnActor<ANaCharacter>( asset->ActorClass );
			m_Actor->Initialize( m_pWorld, this );

			if ( m_ItemProperty ){
				
			}
		}
	}

	m_TurnAction->SetSpeed( m_Speed );

/*	m_Status.HP[0]	= FMath::RandRange( 10, 50 );

	m_bCollidable	= true;


	UWorld*	const	world = m_pWorld->GetUWorld();

	//
	m_Actor	= world->SpawnActor<ANaCharacter>();
	m_Actor->Initialize( m_pWorld, this );
//	m_Actor->SetStaticMesh( m_pWorld->DataAsset->TestMesh );
//	m_Actor->SetSkeletalMesh( m_pWorld->DataAsset->TestMesh3, m_pWorld->DataAsset->TestAnimBP, m_pWorld->DataAsset->TestAnim );
	{
		FNaActorDefinition* adef = m_pWorld->DataAsset->FindActorDefinition( "UnityChan" );
		if ( adef ){
			m_Actor->SetActorDefinition( *adef );
		}
	}
	m_Actor->SetWorldPosition( GetWorldPosition(), true );

	m_Speed	= 100;
	m_TurnAction->SetSpeed( 100 );

	m_Group		= 0;
	m_bNoSave	= true;*/

}

//
void UNaEntityCharacter::OnLeave()
{
	if ( m_Actor ){
		m_Actor->Kill();
		m_Actor	= nullptr;
	}
}

//
void UNaEntityCharacter::SetWorldPosition(const FIntVector& pos)
{
	Super::SetWorldPosition( pos );

	if ( m_Actor ){
		m_Actor->SetWorldPosition( pos, true );
	}
}

//
void UNaEntityCharacter::MoveToWorldPosition(const FIntVector& pos)
{
	Super::SetWorldPosition( pos );

	if ( m_Actor ){
		m_Actor->SetWorldPosition( pos, false );
	}
}

//
void UNaEntityCharacter::Attack()
{
	TArray<UNaEntity*>	targets;

	EnumAttackTarget( targets );

	for ( auto& it : targets ){
		it->TakeDamage( FMath::RandRange( 1, (int32)m_Status.BaseParam.Str + 1 ), this );

	}
}

//
void UNaEntityCharacter::TakeDamage( int32 value, UNaEntity* actor )
{
//	GEngine->AddOnScreenDebugMessage( -1, 1.0f, FColor::Red, FString::Printf( TEXT("Damage %d!"), value ) );

	m_Status.HP	= FMath::Clamp( m_Status.HP - value, 0, m_Status.HPMax );

	if ( value >= 0 ){
		UNaGameLogger::Get()->LogDamage( actor, this, value );
	}

	if ( m_Status.HP <= 0 ){
		UNaGameLogger::Get()->LogDestroy( this );
		Kill();
	}

	if ( m_Actor ){
//		APlayerController*	pc = m_Actor->GetWorld()->GetFirstPlayerController();
//		UNaPopupDamage*		uiPopup;
//		FVector		pos;
//		FVector2D	spos;

//		pc->ProjectWorldLocationToScreen( m_Actor->GetActorLocation(), spos );

//		uiPopup	= CreateWidget<UNaPopupDamage>( m_pWorld->GetWorldContext(), m_pWorld->DataAsset->TestDialogClass );
//		uiPopup->SetPositionInViewport( spos - FVector2D( 128.0f, 128.0f ) );
//		uiPopup->SetDamage( value );
//		uiPopup->AddToViewport();
	}
}

//! アイテム使用
void UNaEntityCharacter::UseItem( UNaItem* item, ENaActionTrigger::Type action )
{
	for ( auto& it : item->GetProperties() ){
		if ( it.Action & (1 << int32(action) ) ){
			switch ( it.Effect ){
			case ENaActionEffect::RecoverHP:
				TakeDamage( -it.Params[0], nullptr );
				break;
			}
		}
	}
}

//! 干渉対象収集
void UNaEntityCharacter::GatherInteractTarget( TArray<UNaEntity*>& outVal )
{
	const FIntVector	c_dir[ENaDirection::ENUM_MAX] = 
	{
		FIntVector( 0, -1, 0 ), FIntVector( 1, -1, 0 ), FIntVector( 1, 0, 0 ), FIntVector( 1, 1, 0 ),
		FIntVector( 0,  1, 0 ), FIntVector( -1, 1, 0 ), FIntVector( -1, 0, 0 ), FIntVector( -1, -1, 0 ),
	};
	FNaWorldBlockWork	src;
	FIntVector	pos = GetWorldPosition() + c_dir[int32(m_Direction)];
	int32		height = 0,minz,maxz;
	TArray<UNaEntity*>	buff;

	if ( m_pWorld->GetBlock( GetWorldPosition(), src ) ){
		height	= src.MetaData.Height[0];
	}

	// 自分の位置-128から+256までの高さ
	minz	= pos.Z + ((height - 128) >> 8);
	maxz	= pos.Z + ((height + 256) >> 8);

	for ( int32 i = minz; i <= maxz; ++i ){
		pos.Z	= i;

		buff.Reset();
		if ( m_pWorld->FindEntity( pos, buff ) ){
			outVal.Append( buff );
		}
	}
}

// 攻撃対象の列挙 //
void UNaEntityCharacter::EnumAttackTarget( TArray<UNaEntity*>& outVal )
{
	const FIntVector	c_dir[ENaDirection::ENUM_MAX] = 
	{
		FIntVector( 0, -1, 0 ), FIntVector( 1, -1, 0 ), FIntVector( 1, 0, 0 ), FIntVector( 1, 1, 0 ),
		FIntVector( 0,  1, 0 ), FIntVector( -1, 1, 0 ), FIntVector( -1, 0, 0 ), FIntVector( -1, -1, 0 ),
	};
	FNaWorldBlockWork	src;
	FIntVector	pos = GetWorldPosition() + c_dir[int32(m_Direction)];
	int32		height = 0,minz,maxz;
	TArray<UNaEntity*>	buff;

	if ( m_pWorld->GetBlock( GetWorldPosition(), src ) ){
		height	= src.MetaData.Height[0];
	}

	// 自分の位置-128から+256までの高さ
	minz	= pos.Z + ((height - 128) >> 8);
	maxz	= pos.Z + ((height + 256) >> 8);

	for ( int32 i = minz; i <= maxz; ++i ){
		pos.Z	= i;

		buff.Reset();
		if ( m_pWorld->FindEntity( pos, buff ) ){
			outVal.Append( buff );
		}
	}
}

//! エンティティ情報生成
void UNaEntityCharacter::CreateFromAsset( const FNaEntityDataAsset& asset )
{
	Super::CreateFromAsset( asset );

	UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
	FNaRaceDataAsset*	race;

	//! 種族データ反映
	race	= alib->FindRaceAsset( asset.Race );
	if ( race ){
		m_Equipments.Reserve( race->EquipParts.Num() );

		for ( auto& it : race->EquipParts ){
			FNaCharacterEquipPart	ep;

			ep.Part	= it;
			ep.Item	= nullptr;
			m_Equipments.Add( ep );
		}
	}

	m_Speed	= 100;
}

//! アイテム装備
void UNaEntityCharacter::EquipItem( int32 index, UNaItem* item )
{
	if ( m_Equipments.IsValidIndex( index ) ){
		if ( m_Equipments[index].Item ){
			m_Equipments[index].Item->SetEquip( false );

//			if ( !item ){
//				UNaGameLogger::Get()->LogEquip( this, m_Equipments[index].Item, false );
//			}
		}

		m_Equipments[index].Item	= item;
		if ( m_Equipments[index].Item ){
			m_Equipments[index].Item->SetEquip( true );

//			UNaGameLogger::Get()->LogEquip( this, item, true );
		}
	}
}

//! アイテム情報セット
void UNaEntityCharacter::SetItemProperty( UNaItem* item )
{
	m_ItemProperty	= item;
}

// ターン開始
void UNaEntityCharacter::OnBeginTurn()
{
	m_SM->ChangeState( EState::Action );
}

// ターン更新
void UNaEntityCharacter::OnUpdateTurn( float DeltaTime )
{
	m_SM->Execute( DeltaTime );

	if ( m_Profile.AIType < 0 ){
		m_TurnAction->EndTurn();
		return;
	}

	if ( m_Manipulator ){
		m_Manipulator->UpdateControl( m_SM, DeltaTime );
	}
}

//! シリアライズ
void UNaEntityCharacter::OnSerialize( FArchive& ar )
{
	enum EModuleFlag
	{
		Item
	};

	Super::OnSerialize( ar );

	int32	moduleFlag = 0;

	if ( ar.IsSaving() ){
		moduleFlag	|= m_ItemProperty ? (1 << EModuleFlag::Item) : 0;
	}
	ar << moduleFlag;

	if ( moduleFlag & (1 << EModuleFlag::Item ) ){
		if ( ar.IsLoading() ){
			m_ItemProperty	= NewObject<UNaItem>();
		}
		m_ItemProperty->Serialize( ar );
		m_ItemProperty->SetOwner( this );
	}
}
