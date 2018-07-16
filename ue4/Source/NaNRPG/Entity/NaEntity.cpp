// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaEntity.h"

#include "World/NaWorld.h"

#include "Entity/Character/NaEntityPlayer.h"
#include "Entity/Env/NaEntitySpawnManager.h"

#include "Assets/NaAssetLibrary.h"
#include "Assets/Entity/NaEntityDataTable.h"

// 
UNaEntity::UNaEntity()
: m_IsAbstract( false )
, m_IsKill( false )
, m_Direction( ENaDirection::South )
, m_Speed( 500 )
, m_ID( -1 )
, m_Stage( ENaEntityStage::Chunk )
{
}

// 
void UNaEntity::Initialize( ENaEntity::Type type )
{
	m_Type	= type;

	m_SM	= NewObject<UNaStateMachine>();

	OnAwake();
}

//
void UNaEntity::SetID( int32 id )
{
	m_ID	= id;
}

//
void UNaEntity::ResetID()
{
	m_ID	= -1;
}

// スポーン時処理
void UNaEntity::Spawn()
{
	OnSpawn();

//	UpdateCurrentChunk();
}

// デスポーン時処理
void UNaEntity::Despawn()
{
	OnDespawn();
}

//
void UNaEntity::Enter()
{
	UpdateCurrentChunk();

	OnEnter();

	m_IsKill	= false;
}

//
void UNaEntity::Leave()
{
	OnLeave();

	if ( m_pCurrentChunk ){
		m_pCurrentChunk->DetachEntity( this );
		m_pCurrentChunk	= nullptr;
	}
	m_pWorld	= nullptr;
}

//! ワールド間移動
void UNaEntity::TravelWorld( FName wid )
{
	m_IsKill	= true;
	SetWorldID( wid );
}

// Naワールド設定
void UNaEntity::SetNaWorld( UNaWorld* world )
{
	m_pWorld	= world;
}

//! エンティティ情報生成
void UNaEntity::CreateFromAsset( const FNaEntityDataAsset& asset )
{
	m_AssetID	= asset.Name;

	m_Profile.CopyFromAsset( asset );

	m_EventID	= asset.DefaultEvent;

	PostLoadProcess( &asset );
}

//! エンティティパラメータ設定
void UNaEntity::SetEntityParam( FName key, FString value )
{
	m_Params[key]	= value;
}

//! エンティティパラメータ設定
void UNaEntity::SetEntityParams( const TMap<FName, FString>& values )
{
	m_Params.Append( values );
}

//! エンティティパラメータ取得
FString UNaEntity::GetEntityParam( FName key )
{
	return m_Params[key];
}

//! イベントセット
void UNaEntity::SetEvent( FName eventID )
{
	if ( !eventID.IsNone() ){
		m_EventID	= eventID;
	}
}

//! イベント取得
bool UNaEntity::FindEvent( ENaEventTrigger trigger, const UNaEventAsset*& outEvent, int32& outSheet )
{
	UNaEventAsset*	asset = UNaAssetLibrary::Get()->FindEventAsset( m_EventID );

	if ( asset ){
		for ( int32 i = asset->Sheets.Num() - 1; i >= 0; --i ){
			const FNaEventSheet&	sheet = asset->Sheets[i];

			if ( sheet.Trigger == trigger ){
				outEvent	= asset;
				outSheet	= i;
				return true;
			}
		}
	}

	return false;
}

//
void UNaEntity::SetWorldPosition(const FIntVector& pos)
{
	m_WorldPos	= pos;
	UpdateCurrentChunk();
}
void UNaEntity::MoveToWorldPosition(const FIntVector& pos)
{
	m_WorldPos	= pos;
	UpdateCurrentChunk();
}

// カレントチャンク更新 //
void UNaEntity::UpdateCurrentChunk()
{
	if ( m_pWorld && !IsIntangible() ){
		UNaChunk*	chunk;

		chunk	= m_pWorld->GetChunkFromWorld( m_WorldPos );

		if ( chunk && chunk != m_pCurrentChunk ){
			if ( m_pCurrentChunk ){
				m_pCurrentChunk->DetachEntity( this );
			}
			m_pCurrentChunk	= chunk;
			m_pCurrentChunk->AttachEntity( this );
		}
	}
}

// 移動先座標取得 //
bool UNaEntity::FindMovePosition( ENaDirection dir, FIntVector& outPos )
{
	const FIntVector	c_dir[8] = 
	{
		FIntVector( 0,-1, 0 ), FIntVector( 1,-1, 0 ), FIntVector( 1, 0, 0 ), FIntVector( 1, 1, 0 ),
		FIntVector( 0, 1, 0 ), FIntVector(-1, 1, 0 ), FIntVector(-1, 0, 0 ), FIntVector(-1,-1, 0 )
	};
	UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
	FNaBlockDataAsset*	block;
	FNaWorldBlockWork	cell;
	FIntVector	wpos = GetWorldPosition();
	FIntVector	pos;
	int32		height,iz;

	//! 現在の高さ算出
	height	= wpos.Z << 8;
	if ( m_pWorld->GetBlock( wpos, cell ) ){
		//! 
		block	= alib->FindBlockAsset( cell.BlockID );
		if ( block && block->Phase == ENaBlockPhase::Solid ){
			height	+= cell.MetaData.Height[0] + 1;
		}
	}

	//! 検索開始位置
	pos		= wpos + c_dir[int32(dir)];
	outPos	= pos;
	iz		= height + 192;

	for ( int32 i = 0; i < 3; ++i ){
		pos.Z	= iz >> 8;

		if ( m_pWorld->GetBlock( pos, cell ) ){
			//! 
			block	= alib->FindBlockAsset( cell.BlockID );
			if ( block && block->Phase == ENaBlockPhase::Solid ){
				int32	tmp;

				tmp	= (pos.Z << 8) + cell.MetaData.Height[0] + 1;

				if ( tmp - height >= 192 ){
					return false;
				}
				else if ( tmp - height >= -128 ){
					outPos		= pos;
					outPos.Z	= tmp >> 8;
					break;
				}
			}
		}
		else {
			return false;
		}
		iz	-= 256;
	}

	// キャラ確認 //
	TArray<UNaEntity*>	buff;

	if ( m_pWorld->FindEntity( outPos, buff ) ){
		for ( auto& it : buff ){
			if ( it->IsCollidable() ){
				return false;
			}
		}
	}
	
	return true;
}

//! 所属チャンク座標取得
FIntVector UNaEntity::GetChunkPosition() const
{
	FIntVector	cpos = m_WorldPos;

	cpos.X	>>= 4;
	cpos.Y	>>= 4;
	cpos.Z	>>= 4;

	return cpos;
}

void UNaEntity::Serialize( FArchive& ar )
{
	OnSerialize( ar );

	//! シリアライズ後処理
	if ( ar.IsLoading() ){
		UNaAssetLibrary*			alib = UNaAssetLibrary::Get();
		const FNaEntityDataAsset*	asset = nullptr;
		
		asset	= alib->FindEntityAsset( m_AssetID );
		if ( asset ){
			PostLoadProcess( asset );
		}
	}
}

//! シリアライズ処理
void UNaEntity::OnSerialize( FArchive& ar )
{
	ar << m_Type;
	SerializeFName( ar, m_AssetID );
	ar << m_ID;
	ar << m_Profile;

	ar << m_Stage;

	ar << m_WorldID;
	ar << m_WorldPos;
	ar << m_Direction;

	ar << m_Speed;
	ar << m_WaitTime;

	ar << m_Group;
}

//! 一時データの更新
void UNaEntity::PostLoadProcess( const FNaEntityDataAsset* asset )
{
	m_Asset			= asset;
	m_Size			= asset->EntitySize;
	m_Collidable	= asset->IsCollidable;
}
