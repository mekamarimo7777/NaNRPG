// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPGEd.h"
#include "SNaMapEditorViewport.h"

#include "Assets/Map/Actions/NaMapEditorActions.h"

//====================================================================================================

//! 
SNaMapEditorViewport::SNaMapEditorViewport()
: m_PreviewScene( FPreviewScene::ConstructionValues() )
, m_World( nullptr )
, m_WorldActor( nullptr )
, m_EditTool( ENaMapPartsEditTool::Tier )
, m_CurrentBlock( nullptr )
, m_BlockSizeZ( 255 )
{
	bCanTick	= true;
}

//! 
SNaMapEditorViewport::~SNaMapEditorViewport()
{
}

//! 
void SNaMapEditorViewport::AddReferencedObjects( FReferenceCollector& Collector )
{
	Collector.AddReferencedObject( m_World );
	Collector.AddReferencedObject( m_WorldActor );
}

//! 
TSharedRef<class SEditorViewport> SNaMapEditorViewport::GetViewportWidget()
{
	return SharedThis(this);
}

//! 
TSharedPtr<FExtender> SNaMapEditorViewport::GetExtenders() const
{
	TSharedPtr<FExtender> Result(MakeShareable(new FExtender));
	return Result;
}

//! 
void SNaMapEditorViewport::OnFloatingButtonClicked()
{
}

//! 
void SNaMapEditorViewport::SetEditorWorld( UNaEditorWorld* naw )
{
	m_World	= naw;
	if ( m_World ){
		m_WorldActor	= GetWorld()->SpawnActor<ANaEditorWorldActor>();
	}
}

//! フロアサイズ設定
void SNaMapEditorViewport::SetFloorSize( FIntVector size )
{
	UStaticMeshComponent*	comp;

	comp	= const_cast<UStaticMeshComponent*>( m_PreviewScene.GetFloorMeshComponent() );
	comp->SetWorldLocation( FVector( size.X * 5.0f, size.Y * 5.0f, -0.5f ) );
	comp->SetWorldScale3D( FVector( size.X * 0.01f, size.Y * 0.01f, 0.1f ) );
}

//! 
void SNaMapEditorViewport::OnHoverMouse( FVector& pos, FVector& dir )
{
}

//! 
void SNaMapEditorViewport::OnClickViewport( FVector& pos, FVector& dir, FKey key )
{
	UWorld*	world = m_PreviewScene.GetWorld();

	{
		FHitResult	hitResult;

		ECollisionChannel	channel = ECC_WorldDynamic;
		FVector				start,end;
	
		start	= pos;
		end		= start + dir * 1000.0f;

		if ( world->LineTraceSingleByChannel( hitResult, start, end, channel ) ){
			FNaWorldBlockWork	block;
			FVector		tpos;
			FIntVector	cellPos;
			int32		bid = -1;

			if ( key == EKeys::LeftMouseButton ){
				tpos	= hitResult.Location - hitResult.Normal * 5.0f;
				bid		= 0;
			}
			else {
				tpos	= hitResult.Location + hitResult.Normal * 5.0f;
				bid		= m_CurrentBlock ? m_CurrentBlock->UID : 0;
			}

			FIntVector	tmin,tmax;

			tmin	= m_World->GetChunkMin() * 16;
			tmax	= m_World->GetChunkMax() * 16 + FIntVector( 15, 15, 15 );

			cellPos.X	= FMath::Clamp( (int32)(tpos.X * 0.1f), tmin.X, tmax.X );
			cellPos.Y	= FMath::Clamp( (int32)(tpos.Y * 0.1f), tmin.Y, tmax.Y );
			cellPos.Z	= FMath::Clamp( (int32)(tpos.Z * 0.1f), tmin.Z, tmax.Z );

			if ( m_World->GetBlock( cellPos, block ) ){
				if ( block.BlockID != bid ){
					block.BlockID	= bid;
					block.MetaData.Height[0]	= m_BlockSizeZ;
					block.MetaData.Height[1]	= m_BlockSizeZ;
					block.MetaData.Height[2]	= m_BlockSizeZ;
					block.MetaData.Height[3]	= m_BlockSizeZ;

					m_World->SetBlock( cellPos, block );

					cellPos.X	>>= 4;
					cellPos.Y	>>= 4;
					cellPos.Z	>>= 4;
					m_WorldActor->UpdateChunk( cellPos );
				}
			}

			//DrawDebugSphere( world, tpos, 10.0f, 16, FColor::Red, false, 3.0f );
		}
	}
}

//====================================================================================================

//! 
void SNaMapEditorViewport::Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime )
{
	UWorld*	world = m_PreviewScene.GetWorld();

	switch ( m_EditTool ){
	case ENaMapPartsEditTool::Tier:
		break;
	}
}

//! 
TSharedRef<FEditorViewportClient> SNaMapEditorViewport::MakeEditorViewportClient()
{
	m_ViewportClient = MakeShareable( new FNaMapEditorViewportClient( SharedThis(this), m_PreviewScene ) );

	m_ViewportClient->bSetListenerPosition = false;

	m_ViewportClient->SetRealtime( true );
//	m_ViewportClient->VisibilityDelegate.BindSP( this, &StaticMeshEditorViewport::IsVisible );

	return m_ViewportClient.ToSharedRef();
}

//! 
void SNaMapEditorViewport::BindCommands()
{
	SEditorViewport::BindCommands();

	const FNaMapEditorCommands&	Commands = FNaMapEditorCommands::Get();

	//! 
	CommandList->MapAction(
		Commands.SelectTierTool,
		FExecuteAction::CreateSP( this, &SNaMapEditorViewport::SelectTierTool ),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP( this, &SNaMapEditorViewport::IsSelectTierToolChecked ) );

	//! 
	CommandList->MapAction(
		Commands.SelectMinecraftTool,
		FExecuteAction::CreateSP( this, &SNaMapEditorViewport::SelectMinecraftTool ),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP( this, &SNaMapEditorViewport::IsSelectMinecraftToolChecked ) );
}

//! 
void SNaMapEditorViewport::SelectTierTool()
{
	m_EditTool	= ENaMapPartsEditTool::Tier;
}
//! 
bool SNaMapEditorViewport::IsSelectTierToolChecked() const
{
	return m_EditTool == ENaMapPartsEditTool::Tier;
}


//! 
void SNaMapEditorViewport::SelectMinecraftTool()
{
	m_EditTool	= ENaMapPartsEditTool::MinecraftLike;
}
//! 
bool SNaMapEditorViewport::IsSelectMinecraftToolChecked() const
{
	return m_EditTool == ENaMapPartsEditTool::MinecraftLike;
}
