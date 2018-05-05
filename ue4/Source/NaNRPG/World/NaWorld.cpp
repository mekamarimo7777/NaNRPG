// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaWorld.h"

#include "Actor/Entity/NaActorBase.h"

#include "Assets/NaAssetLibrary.h"
#include "Assets/Map/NaBlockDataTable.h"

#include "Entity/INaEntityFactory.h"

#include "Database/NaGameDatabase.h"

//
UNaWorld::UNaWorld()
: m_CurrentChunkPos( 0, 0, SHORT_MAX )
, m_ChunkRange( 9, 9, 9 )
, m_ChunkMin( -INT_MAX, -INT_MAX, 0 )
, m_ChunkMax( INT_MAX, INT_MAX, 15 )
, m_NextEntityID( 0 )
{
	m_Generator	= NewObject<UNaWorldGenerator>();
}

//! 新規ワールド生成
UNaWorld* UNaWorld::Create( FName uid, FName assetID )
{
	UNaWorld*	world	= NewObject<UNaWorld>();

	world->CreateWorld( uid, assetID );

	return world;
}

//! ワールドデータオープン
UNaWorld* UNaWorld::Open( uint32 dataID )
{
	UNaWorld*	world	= NewObject<UNaWorld>();

	if ( !world->OpenWorld( dataID ) ){
		world	= nullptr;
	}

	return world;
}

//! 使用開始準備
void UNaWorld::Setup( UWorld* world )
{
	m_WorldContext	= world;
}

//! 
void UNaWorld::Update( float DeltaTime )
{
	UNaGameDatabase*	db = UNaGameDatabase::GetDB();

	if ( UNaEntity* player = db->GetPlayer() ){
		if ( player->GetWorldID() == GetUID() ){
			SetCurrentPosition( player->GetWorldPosition() );
		}
	}

	//! ターン進行
	AdvanceTurn();
}

//! ワールドデータ構築
bool UNaWorld::CreateWorld( FName uid, FName assetID )
{
	UNaGameDatabase*	db = UNaGameDatabase::GetDB();
	UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
	UNaWorldAsset*		asset;

	m_UID		= uid;
	m_DataID	= db->GenerateWorldDataID();

	//! アセットID未指定はUIDで検索
	if ( assetID.IsNone() ){
		assetID	= uid;
	}

	//! アセットから構築
	asset	= alib->FindWorldAsset( assetID );

	if ( asset ){
		m_DisplayName	= asset->DisplayName;

		//! マップ作成
		for ( auto& it : asset->MapEntries ){
			CreateMap( it.Location, it.Map );
		}

		// ワールド定義エンティティ生成
		for ( auto& it : asset->Entities ){
			const FNaEntityDataAsset*	entAsset;

			entAsset	= alib->FindEntityAsset( it.EntityID );
			if ( entAsset ){
				UNaEntity*	entity = INaEntityFactory::NewEntity<UNaEntity>( entAsset->Type );

				entity->CreateFromAsset( *entAsset );
				entity->SetWorldPosition( it.Position );
				entity->SetEvent( it.EventID );
				entity->SetEntityParameter( it.Params );

				switch ( it.Stage ){
				case ENaEntityStage::World:
					RegisterEntity( entity );
					break;
				case ENaEntityStage::Global:
					db->RegisterGlobalEntity( entity );
					break;
				case ENaEntityStage::Chunk:
					//! ワールド定義ではチャンク管理エンティティは作れない
					//! もしくは一時スポーン待ちリストに入れておく？
					check( false );
					break;
				}
			}
		}
	}

	//! 保存処理
	{
		IPlatformFile&	pf = FPlatformFileManager::Get().GetPlatformFile();
		FString			dir,region_dir,map_dir,fname;
		FBufferArchive	ar;

		dir	= MakeWorldDirPath( m_DataID );
		if ( FPaths::DirectoryExists( dir ) ){
			pf.DeleteDirectoryRecursively( *dir );
		}
		pf.CreateDirectoryTree( *dir );
		m_WorldPath	= dir;

		// リージョンフォルダ生成
		region_dir	= GetRegionDirPath();
		if ( !FPaths::DirectoryExists( region_dir ) ){
			pf.CreateDirectoryTree( *region_dir );
		}

		// マップフォルダ生成
		map_dir	= GetMapDirPath();
		if ( !FPaths::DirectoryExists( region_dir ) ){
			pf.CreateDirectoryTree( *region_dir );
		}

		fname	= GetWorldFilePath();

		Serialize( ar );
		FFileHelper::SaveArrayToFile( ar, *fname );
	}

	return true;
}

//
bool UNaWorld::OpenWorld( int32 dataID )
{
	FString	dir,fname;

	dir	= MakeWorldDirPath( dataID );
	if ( !FPaths::DirectoryExists( dir ) ){
		return false;
	}
	m_WorldPath	= dir;

	fname	= GetWorldFilePath();
	if ( FPaths::DirectoryExists( fname ) ){
		return false;
	}
	if ( !FPaths::FileExists( fname ) ){
		return false;
	}

	TArray<uint8>	buff;

	if ( FFileHelper::LoadFileToArray( buff, *fname ) ){
		FMemoryReader	reader( buff );

		Serialize( reader );

		// 無形エンティティを登録
/*		for( auto& it : m_Entities ){
			if ( it->IsIntangible() ){
				EnterEntity( it );
			}
		}*/
	}

	return true;
}

//
void UNaWorld::CloseWorld( bool isSave )
{
	//! プレイヤー・パーティメンバーをワールドから外す
	{
		UNaGameDatabase*	db = UNaGameDatabase::GetDB();

		DespawnEntity( db->GetPlayer() );

	}

	for ( auto it : m_Regions ){
		it->CloseRegion();
	}
	m_Regions.Empty();
	m_RegionMap.Empty();
}

//
void UNaWorld::Serialize( FArchive& ar )
{
	//! ID関連
	ar << m_UID;
	ar << m_AssetID;
	ar << m_DataID;

	//! 
	ar << m_DisplayName;
	
	//! 
	ar << m_NextEntityID;

	//! 
	INaEntityFactory::SerializeEntities( ar, m_Entities );
}

//! 
void UNaWorld::SetChunkRange( FIntVector range )
{
	m_ChunkRange	= range;
}

// 
void UNaWorld::SetCurrentPosition( const FIntVector& pos )
{
	FIntVector	cpos_prev;

	m_CurrentWorldPos	= pos;

	cpos_prev			= m_CurrentChunkPos;
	m_CurrentChunkPos.X	= m_CurrentWorldPos.X >> 4;
	m_CurrentChunkPos.Y	= m_CurrentWorldPos.Y >> 4;
	m_CurrentChunkPos.Z	= m_CurrentWorldPos.Z >> 4;

	if ( m_CurrentChunkPos != cpos_prev ){
		UpdateWorld();
	}

	//! 天井セル計算
	{
		FIntVector	tpos;
		int32		cz;

		if ( FindCeilPos( pos, tpos ) ){
			cz	= pos.Z + 3;
		}
		else {
			cz	= SHORT_MAX;
		}

		if ( cz != m_CeilZ ){
			SetCeilZ( cz );
		}
	}
}

//! アクティブチャンクの更新
void UNaWorld::UpdateWorld()
{
	TArray<UNaChunk*>	chunks;
	TArray<UNaChunk*>	new_chunks;
	UNaRegion*			region;
	UNaChunk*			chunk;
	FIntVector			tpos,rpos;

	tpos.X	= m_CurrentChunkPos.X - (m_ChunkRange.X >> 1);

	for ( int32 ix = 0; ix < m_ChunkRange.X; ++ix, ++tpos.X ){
		if ( tpos.X < m_ChunkMin.X || tpos.X > m_ChunkMax.X ){
			continue;
		}
		//
		tpos.Y	= m_CurrentChunkPos.Y - (m_ChunkRange.Y >> 1);

		for ( int32 iy = 0; iy < m_ChunkRange.Y; ++iy, ++tpos.Y ){
			if ( tpos.Y < m_ChunkMin.Y || tpos.Y > m_ChunkMax.Y ){
				continue;
			}
			//
			tpos.Z	= m_CurrentChunkPos.Z - (m_ChunkRange.Z >> 1);

			for ( int32 iz = 0; iz < m_ChunkRange.Z; ++iz, ++tpos.Z ){
				if ( tpos.Z < m_ChunkMin.Z || tpos.Z > m_ChunkMax.Z ){
					continue;
				}

				//! チャンク検出
				chunk	= m_ChunkMap.FindRef( tpos );

				if ( chunk ){
					chunks.Add( chunk );
					m_ChunkMap.Remove( tpos );
				}
				else  {
					//
					rpos.X	= tpos.X >> 4;
					rpos.Y	= tpos.Y >> 4;
					rpos.Z	= tpos.Z >> 4;
					region	= m_RegionMap.FindRef( rpos );

					if ( !region ){
						FString	regionPath = FPaths::Combine( *m_WorldPath, TEXT( "regions" ) );

						region	= NewObject<UNaRegion>();
						region->Initialize( this, rpos );
						region->Open();

						m_Regions.Add( region );
						m_RegionMap.Add( rpos, region );
					}

					chunk	= region->GetChunk( tpos );
					if ( !chunk ){
						chunk	= region->CreateChunk( tpos );
					}
					new_chunks.Add( chunk );
				}
			}
		}
	}
	
	// 範囲外チャンククローズ //
	for ( auto it : m_ChunkMap ){
		region = it.Value->GetRegion();
		region->CloseChunk( it.Value->GetPosition() );
	}
	m_ChunkMap.Reset();

	//! 既存チャンクセット
	for ( auto it : chunks ){
		m_ChunkMap.Add( it->GetPositionInWorld(), it );
	}

	//! 新規チャンクオープン
	{
		TArray<UNaEntity*>	entities;

		for ( auto it : new_chunks ){
			m_ChunkMap.Add( it->GetPositionInWorld(), it );
			it->Open();

			//! 上位管理エンティティの生成
			GatherWorldEntities( it->GetPositionInWorld(), entities );
			for ( auto& it2 : entities ){
				SpawnEntity( it2, it2->GetWorldPosition() );
			}
		}
	}

	// 空リージョンクローズ //
	for ( int32 i = m_Regions.Num() - 1; i >= 0; --i ){
		region = m_Regions[i];

		if ( region->GetActiveChunkCount() == 0 ){
			region->Close();
			m_RegionMap.Remove( region->GetRegionPos() );
			m_Regions.RemoveAt( i );
		}
	}

	// 作業用マップクローズ //
	for ( auto& it : m_MapData ){
		it.Value->CloseMap();
	}
}

//! ターン進行
void UNaWorld::AdvanceTurn()
{
	while ( true ){
		// 削除待ちエンティティ削除
		for ( int32 i = m_SpawnEntities.Num() - 1; i >= 0; --i ){
			if ( m_SpawnEntities[i]->IsPendingKill() ){
				DespawnEntity( m_SpawnEntities[i] );
			}
		}

		// ターンアクション開始
		if ( !m_CurrentAction && m_ActionChain.Num() > 0 ){
			m_CurrentAction	= m_ActionChain[0];
			m_ActionChain.RemoveAt( 0 );

			for ( auto it : m_ActionChain ){
				it->DecreaseWaitTime( m_CurrentAction->GetWaitTime() );
			}
			m_CurrentAction->BeginTurn();
		}

		// ターン処理
		if ( m_CurrentAction ){
			m_CurrentAction->ExecuteTurn( 0.0f );

			// 終了しなかった場合は次フレームへ
			if ( !m_CurrentAction->IsEndTurn() ){
				break;
			}

			InsertActionChain( m_CurrentAction );
			m_CurrentAction	= nullptr;
		}
		else {
			break;
		}
	}
}

//
void UNaWorld::InsertActionChain( UNaTurnActionComponent* tac )
{
	int32	i;

	tac->ResetWaitTime();

	for ( i = 0; i < m_ActionChain.Num(); ++i ){
		if ( tac->GetWaitTime() < m_ActionChain[i]->GetWaitTime() ){
			break;
		}
	}

	m_ActionChain.Insert( tac, i );
}

//
UNaRegion* UNaWorld::GetRegion( const FIntVector& worldPos )
{
	return nullptr;
}

// 
UNaChunk* UNaWorld::GetChunk( const FIntVector& chunkPos )
{
	return m_ChunkMap.FindRef( chunkPos );
}
//
UNaChunk* UNaWorld::GetChunkFromWorld( FIntVector worldPos )
{
	worldPos.X	>>= 4;
	worldPos.Y	>>= 4;
	worldPos.Z	>>= 4;
	return GetChunk( worldPos );
}

//! ブロック設定
void UNaWorld::SetBlock( FIntVector pos, FNaWorldBlockWork& block )
{
	UNaChunk*	chunk;
	FIntVector	cpos;

	cpos.X	= pos.X >> 4;
	cpos.Y	= pos.Y >> 4;
	cpos.Z	= pos.Z >> 4;

	chunk	= GetChunk( cpos );
	if ( chunk ){
		pos.X	-= cpos.X << 4;
		pos.Y	-= cpos.Y << 4;
		pos.Z	-= cpos.Z << 4;
		chunk->SetBlock( pos, block );
	}
}

//! ブロック取得
bool UNaWorld::GetBlock( FIntVector worldPos, FNaWorldBlockWork& outVal )
{
	UNaChunk*	chunk;
	FIntVector	cpos;

	cpos.X	= worldPos.X >> 4;
	cpos.Y	= worldPos.Y >> 4;
	cpos.Z	= worldPos.Z >> 4;

	chunk	= GetChunk( cpos );
	if ( chunk ){
		worldPos.X	-= cpos.X << 4;
		worldPos.Y	-= cpos.Y << 4;
		worldPos.Z	-= cpos.Z << 4;

		if ( chunk->GetBlock( worldPos, outVal ) ){
			return true;
		}
	}

	return false;
}

// 接地セル検索 //
bool UNaWorld::FindGroundPos( FIntVector startPos, FIntVector& outPos )
{
	UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
	FNaBlockDataAsset*	block;
	FNaWorldBlockWork	cell;
	int32	pz = -INT_MAX;

	while ( GetBlock( startPos, cell ) ){
		block	= alib->FindBlockAsset( cell.BlockID );
		if ( block && block->Phase == ENaBlockPhase::Solid ){
			pz			= (startPos.Z << 8) + cell.MetaData.Height[0] + 1;
			outPos		= startPos;
			outPos.Z	= pz >> 8;
			return true;
		}
		startPos.Z--;
	}

	// 見つからなかったので下限座標を設定 //
	outPos	= startPos;
	outPos.Z++;

	return true;
}

// 天井セル検索
bool UNaWorld::FindCeilPos( FIntVector startPos, FIntVector& outPos )
{
	UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
	FNaBlockDataAsset*	block;
	FNaWorldBlockWork	cell;

	startPos.Z++;

	while ( GetBlock( startPos, cell ) ){
		block	= alib->FindBlockAsset( cell.BlockID );
		if ( block && block->Phase == ENaBlockPhase::Solid ){
			outPos	= startPos;
			return true;
		}
		startPos.Z++;
	}

	// 見つからなかったので上限座標を設定 //
	outPos	= startPos;

	return false;
}

//! リージョンデータ読み込み
bool UNaWorld::ReadRegionData( FIntVector pos, TArray<uint8>& outVal )
{
	FString	regionPath = FPaths::Combine( *m_WorldPath, TEXT( "regions" ) );
	FString	filePath = FPaths::Combine( *regionPath, *FString::Printf( TEXT("r%d,%d,%d"), pos.X, pos.Y, pos.Z ) );

	if ( FPaths::FileExists( filePath ) ){
		return FFileHelper::LoadFileToArray( outVal, *filePath );
	}

	return false;
}
//! リージョンデータ書き込み
bool UNaWorld::WriteRegionData( FIntVector pos, const TArray<uint8>& inVal )
{
	FString	regionPath = FPaths::Combine( *m_WorldPath, TEXT( "regions" ) );

	if ( FPaths::DirectoryExists( regionPath ) ){
		FString	filePath = FPaths::Combine( *regionPath, *FString::Printf( TEXT("r%d,%d,%d"), pos.X, pos.Y, pos.Z ) );

		FFileHelper::SaveArrayToFile( inVal, *filePath );

		return true;
	}

	return false;
}

//
bool UNaWorld::FindEntity( FIntVector worldPos, TArray<UNaEntity*>& outVal )
{
	for ( auto& it : m_SpawnEntities ){
		if ( it->GetWorldPosition() == worldPos ){
			outVal.Add( it );
		}
	}
	return outVal.Num() > 0;
}

// エンティティ管理登録
bool UNaWorld::RegisterEntity( UNaEntity* entity )
{
	if ( m_Entities.Contains( entity ) ){
		return false;
	}

	entity->SetStage( ENaEntityStage::World );
	entity->SetID( IssueEntityID() );
	m_Entities.Add( entity );

	return true;
}

// エンティティ管理除去
void UNaWorld::UnregisterEntity( UNaEntity* entity )
{
	if ( m_Entities.Contains( entity ) ){
		m_Entities.RemoveSwap( entity );
		entity->SetStage( ENaEntityStage::Chunk );
	}
}

//! エンティティID発行
uint32 UNaWorld::IssueEntityID()
{
	return m_NextEntityID++;
}

// エンティティスポーン
bool UNaWorld::SpawnEntity( UNaEntity* entity, FIntVector pos )
{
	if ( m_SpawnEntities.Contains( entity ) ){
		return false;
	}

	// スポーン処理
	entity->SetNaWorld( this );
	entity->SetWorldPosition( pos );
	entity->Spawn();

	//! 表示
	EnterEntity( entity );

	return true;
}

// エンティティデスポーン
void UNaWorld::DespawnEntity( UNaEntity* entity )
{
	if ( !m_SpawnEntities.Contains( entity ) ){
		return;
	}

	//! 消去
	LeaveEntity( entity );

	//! デスポーン処理
	entity->Despawn();
}

// 
bool UNaWorld::EnterEntity( UNaEntity* entity )
{
	if ( m_SpawnEntities.Contains( entity ) ){
		return false;
	}

	//! スポーンリストへ追加
	m_SpawnEntities.Add( entity );

	//! 実行チェインに追加
	AttachActionChain( entity );

	// 
	entity->SetNaWorld( this );
	entity->Enter();

	// test
	if ( entity->GetType() == ENaEntity::Player ){
		m_pPlayer	= Cast<UNaEntityPlayer>( entity );
	}

	return true;
}

// 
void UNaWorld::LeaveEntity( UNaEntity* entity )
{
	if ( !m_SpawnEntities.Contains( entity ) ){
		return;
	}

	//
	entity->Leave();

	//! アクションチェインから除去
	DetachActionChain( entity );

	//! スポーンリストから除去
	m_SpawnEntities.RemoveSwap( entity );
}

//! アクションチェインに追加
void UNaWorld::AttachActionChain( UNaEntity* entity )
{
	if ( entity->HasTurnAction() ){
		InsertActionChain( entity->GetTurnAction() );
	}
}

//! アクションチェインから除去
void UNaWorld::DetachActionChain( UNaEntity* entity )
{
	if ( entity->HasTurnAction() ){
		m_ActionChain.Remove( entity->GetTurnAction() );
	}
}

//! マップ登録
void UNaWorld::CreateMap( FIntVector location, const UNaMapAsset* mapAsset )
{
	FNaMapEntry	entry;
	UNaMap*		map;

	entry.MapID		= m_MapEntries.Num();
	entry.Location	= location;
	entry.Range		= mapAsset->m_MapSize;
	m_MapEntries.Add( entry );

	map	= NewObject<UNaMap>();
	map->Create( this, entry.MapID, entry.Location, mapAsset );
	map->SaveMap( GetMapDirPath() );
	m_MapData.Add( entry.MapID, map );
}
void UNaWorld::CreateMap( FIntVector location, const FStringAssetReference& mapAsset )
{
	TAssetPtr<UNaMapAsset>	asset = TAssetPtr<UNaMapAsset>( mapAsset );

	asset.LoadSynchronous();
	CreateMap( location, asset.Get() );
}

//! 
void UNaWorld::FindMap( FIntVector location, TArray<UNaMap*>& outArray )
{
	outArray.Reset();

	for ( auto& it : m_MapEntries ){
		if ( location.X >= it.Location.X && location.X < it.Location.X + it.Range.X &&
			 location.Y >= it.Location.Y && location.Y < it.Location.Y + it.Range.Y &&
			 location.Z >= it.Location.Z && location.Z < it.Location.Z + it.Range.Z )
		{
			UNaMap*	map = m_MapData.FindRef( it.MapID );

			if ( !map ){
				map	= NewObject<UNaMap>();
				map->LoadMap( GetMapDirPath(), it.MapID );
			}
			outArray.Add( map );
		}
	}
}

//! 指定チャンクのエンティティ収集
void UNaWorld::GatherWorldEntities( const FIntVector& chunkPos, TArray<UNaEntity*>& outVal )
{
	outVal.Reset();

	//! グローバルエンティティ検索
	if ( UNaGameDatabase* db = UNaGameDatabase::GetDB() ){
		TArray<UNaEntity*>	entities;

		db->GatherEntities( m_UID, entities );

		for ( auto& it : entities ){
			//! 生存判定
			if ( !it->IsAlive() ){
				continue;
			}

			//! チャンク内・または無形エンティティ
			if ( it->GetChunkPosition() == chunkPos || it->IsIntangible() ){
				outVal.Add( it );
			}
		}
	}

	//! ワールドエンティティ検索
	for ( auto& it : m_Entities ){
		//! 生存判定
		if ( !it->IsAlive() ){
			continue;
		}

		//! チャンク内・または無形エンティティ
		if ( it->GetChunkPosition() == chunkPos || it->IsIntangible() ){
			outVal.Add( it );
		}
	}
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//
FString UNaWorld::MakeWorldDirPath( int32 worldID ) const
{
	return FPaths::Combine( *UNaGameDatabase::GameSavedDir(), *FString::Printf( TEXT("worlds/world%d"), worldID ) );
}

//
FString UNaWorld::GetWorldFilePath() const
{
	return FPaths::Combine( *m_WorldPath, TEXT( "data.nwld" ) );
}

//
FString UNaWorld::GetRegionDirPath() const
{
	return FPaths::Combine( *m_WorldPath, TEXT( "regions" ) );
}

//
FString UNaWorld::GetMapDirPath() const
{
	return FPaths::Combine( *m_WorldPath, TEXT( "maps" ) );
}
