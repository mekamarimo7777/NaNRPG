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

//! �V�K���[���h����
UNaWorld* UNaWorld::Create( FName uid, FName assetID )
{
	UNaWorld*	world	= NewObject<UNaWorld>();

	world->CreateWorld( uid, assetID );

	return world;
}

//! ���[���h�f�[�^�I�[�v��
UNaWorld* UNaWorld::Open( uint32 dataID )
{
	UNaWorld*	world	= NewObject<UNaWorld>();

	if ( !world->OpenWorld( dataID ) ){
		world	= nullptr;
	}

	return world;
}

//! �g�p�J�n����
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

	//! �^�[���i�s
	AdvanceTurn();
}

//! ���[���h�f�[�^�\�z
bool UNaWorld::CreateWorld( FName uid, FName assetID )
{
	UNaGameDatabase*	db = UNaGameDatabase::GetDB();
	UNaAssetLibrary*	alib = UNaAssetLibrary::Get();
	UNaWorldAsset*		asset;

	m_UID		= uid;
	m_DataID	= db->GenerateWorldDataID();

	//! �A�Z�b�gID���w���UID�Ō���
	if ( assetID.IsNone() ){
		assetID	= uid;
	}

	//! �A�Z�b�g����\�z
	asset	= alib->FindWorldAsset( assetID );

	if ( asset ){
		m_DisplayName	= asset->DisplayName;

		//! �}�b�v�쐬
		for ( auto& it : asset->MapEntries ){
			CreateMap( it.Location, it.Map );
		}

		// ���[���h��`�G���e�B�e�B����
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
					//! ���[���h��`�ł̓`�����N�Ǘ��G���e�B�e�B�͍��Ȃ�
					//! �������͈ꎞ�X�|�[���҂����X�g�ɓ���Ă����H
					check( false );
					break;
				}
			}
		}
	}

	//! �ۑ�����
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

		// ���[�W�����t�H���_����
		region_dir	= GetRegionDirPath();
		if ( !FPaths::DirectoryExists( region_dir ) ){
			pf.CreateDirectoryTree( *region_dir );
		}

		// �}�b�v�t�H���_����
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

		// ���`�G���e�B�e�B��o�^
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
	//! �v���C���[�E�p�[�e�B�����o�[�����[���h����O��
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
	//! ID�֘A
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

	//! �V��Z���v�Z
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

//! �A�N�e�B�u�`�����N�̍X�V
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

				//! �`�����N���o
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
	
	// �͈͊O�`�����N�N���[�Y //
	for ( auto it : m_ChunkMap ){
		region = it.Value->GetRegion();
		region->CloseChunk( it.Value->GetPosition() );
	}
	m_ChunkMap.Reset();

	//! �����`�����N�Z�b�g
	for ( auto it : chunks ){
		m_ChunkMap.Add( it->GetPositionInWorld(), it );
	}

	//! �V�K�`�����N�I�[�v��
	{
		TArray<UNaEntity*>	entities;

		for ( auto it : new_chunks ){
			m_ChunkMap.Add( it->GetPositionInWorld(), it );
			it->Open();

			//! ��ʊǗ��G���e�B�e�B�̐���
			GatherWorldEntities( it->GetPositionInWorld(), entities );
			for ( auto& it2 : entities ){
				SpawnEntity( it2, it2->GetWorldPosition() );
			}
		}
	}

	// �󃊁[�W�����N���[�Y //
	for ( int32 i = m_Regions.Num() - 1; i >= 0; --i ){
		region = m_Regions[i];

		if ( region->GetActiveChunkCount() == 0 ){
			region->Close();
			m_RegionMap.Remove( region->GetRegionPos() );
			m_Regions.RemoveAt( i );
		}
	}

	// ��Ɨp�}�b�v�N���[�Y //
	for ( auto& it : m_MapData ){
		it.Value->CloseMap();
	}
}

//! �^�[���i�s
void UNaWorld::AdvanceTurn()
{
	while ( true ){
		// �폜�҂��G���e�B�e�B�폜
		for ( int32 i = m_SpawnEntities.Num() - 1; i >= 0; --i ){
			if ( m_SpawnEntities[i]->IsPendingKill() ){
				DespawnEntity( m_SpawnEntities[i] );
			}
		}

		// �^�[���A�N�V�����J�n
		if ( !m_CurrentAction && m_ActionChain.Num() > 0 ){
			m_CurrentAction	= m_ActionChain[0];
			m_ActionChain.RemoveAt( 0 );

			for ( auto it : m_ActionChain ){
				it->DecreaseWaitTime( m_CurrentAction->GetWaitTime() );
			}
			m_CurrentAction->BeginTurn();
		}

		// �^�[������
		if ( m_CurrentAction ){
			m_CurrentAction->ExecuteTurn( 0.0f );

			// �I�����Ȃ������ꍇ�͎��t���[����
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

//! �u���b�N�ݒ�
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

//! �u���b�N�擾
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

// �ڒn�Z������ //
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

	// ������Ȃ������̂ŉ������W��ݒ� //
	outPos	= startPos;
	outPos.Z++;

	return true;
}

// �V��Z������
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

	// ������Ȃ������̂ŏ�����W��ݒ� //
	outPos	= startPos;

	return false;
}

//! ���[�W�����f�[�^�ǂݍ���
bool UNaWorld::ReadRegionData( FIntVector pos, TArray<uint8>& outVal )
{
	FString	regionPath = FPaths::Combine( *m_WorldPath, TEXT( "regions" ) );
	FString	filePath = FPaths::Combine( *regionPath, *FString::Printf( TEXT("r%d,%d,%d"), pos.X, pos.Y, pos.Z ) );

	if ( FPaths::FileExists( filePath ) ){
		return FFileHelper::LoadFileToArray( outVal, *filePath );
	}

	return false;
}
//! ���[�W�����f�[�^��������
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

// �G���e�B�e�B�Ǘ��o�^
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

// �G���e�B�e�B�Ǘ�����
void UNaWorld::UnregisterEntity( UNaEntity* entity )
{
	if ( m_Entities.Contains( entity ) ){
		m_Entities.RemoveSwap( entity );
		entity->SetStage( ENaEntityStage::Chunk );
	}
}

//! �G���e�B�e�BID���s
uint32 UNaWorld::IssueEntityID()
{
	return m_NextEntityID++;
}

// �G���e�B�e�B�X�|�[��
bool UNaWorld::SpawnEntity( UNaEntity* entity, FIntVector pos )
{
	if ( m_SpawnEntities.Contains( entity ) ){
		return false;
	}

	// �X�|�[������
	entity->SetNaWorld( this );
	entity->SetWorldPosition( pos );
	entity->Spawn();

	//! �\��
	EnterEntity( entity );

	return true;
}

// �G���e�B�e�B�f�X�|�[��
void UNaWorld::DespawnEntity( UNaEntity* entity )
{
	if ( !m_SpawnEntities.Contains( entity ) ){
		return;
	}

	//! ����
	LeaveEntity( entity );

	//! �f�X�|�[������
	entity->Despawn();
}

// 
bool UNaWorld::EnterEntity( UNaEntity* entity )
{
	if ( m_SpawnEntities.Contains( entity ) ){
		return false;
	}

	//! �X�|�[�����X�g�֒ǉ�
	m_SpawnEntities.Add( entity );

	//! ���s�`�F�C���ɒǉ�
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

	//! �A�N�V�����`�F�C�����珜��
	DetachActionChain( entity );

	//! �X�|�[�����X�g���珜��
	m_SpawnEntities.RemoveSwap( entity );
}

//! �A�N�V�����`�F�C���ɒǉ�
void UNaWorld::AttachActionChain( UNaEntity* entity )
{
	if ( entity->HasTurnAction() ){
		InsertActionChain( entity->GetTurnAction() );
	}
}

//! �A�N�V�����`�F�C�����珜��
void UNaWorld::DetachActionChain( UNaEntity* entity )
{
	if ( entity->HasTurnAction() ){
		m_ActionChain.Remove( entity->GetTurnAction() );
	}
}

//! �}�b�v�o�^
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

//! �w��`�����N�̃G���e�B�e�B���W
void UNaWorld::GatherWorldEntities( const FIntVector& chunkPos, TArray<UNaEntity*>& outVal )
{
	outVal.Reset();

	//! �O���[�o���G���e�B�e�B����
	if ( UNaGameDatabase* db = UNaGameDatabase::GetDB() ){
		TArray<UNaEntity*>	entities;

		db->GatherEntities( m_UID, entities );

		for ( auto& it : entities ){
			//! ��������
			if ( !it->IsAlive() ){
				continue;
			}

			//! �`�����N���E�܂��͖��`�G���e�B�e�B
			if ( it->GetChunkPosition() == chunkPos || it->IsIntangible() ){
				outVal.Add( it );
			}
		}
	}

	//! ���[���h�G���e�B�e�B����
	for ( auto& it : m_Entities ){
		//! ��������
		if ( !it->IsAlive() ){
			continue;
		}

		//! �`�����N���E�܂��͖��`�G���e�B�e�B
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
