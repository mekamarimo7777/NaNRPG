// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPGEd.h"
#include "NaMapPartsEditorViewportClient.h"

#include "SNaMapPartsEditorViewport.h"

//!
FNaMapPartsEditorViewportClient::FNaMapPartsEditorViewportClient( const TSharedRef<SNaMapPartsEditorViewport>& InEditorViewport, FAdvancedPreviewScene& InPreviewScene )
: FEditorViewportClient( nullptr, &InPreviewScene, StaticCastSharedRef<SEditorViewport>( InEditorViewport ) )
, m_owner( InEditorViewport )
, m_bDrawing( false )
{
	SetCameraSpeedSetting(2);
}

FNaMapPartsEditorViewportClient::~FNaMapPartsEditorViewportClient()
{
}

//! 
void FNaMapPartsEditorViewportClient::Tick( float DeltaSeconds )
{
	FEditorViewportClient::Tick( DeltaSeconds );

	// Tick the preview scene world.
	if ( !GIntraFrameDebuggingGameThread ){
		PreviewScene->GetWorld()->Tick( LEVELTICK_All, DeltaSeconds );
	}
}

//! 
bool FNaMapPartsEditorViewportClient::InputKey(FViewport* InViewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad )
{
	bool	bHandled = false;
/*
	if ( Event == IE_Pressed ){
		if ( Key == EKeys::LeftMouseButton || Key == EKeys::RightMouseButton ){
			const int32 HitX = InViewport->GetMouseX();
			const int32 HitY = InViewport->GetMouseY();
			FVector	pos, dir;

			CalcWorldPosition( InViewport, HitX, HitY, pos, dir );
			m_owner.Pin()->OnClickViewport( pos, dir, Key );

			bHandled	= true;
		}
	}
	else if ( Event == IE_Released ){
		if ( Key == EKeys::LeftMouseButton ){
			m_bDrawing	= false;
			bHandled	= true;
		}
	}*/

	return bHandled ? true : FEditorViewportClient::InputKey( InViewport, ControllerId, Key, Event, AmountDepressed, bGamepad );
}

//! 
void FNaMapPartsEditorViewportClient::UpdateMouseDelta()
{
	if ( m_bDrawing ){
		return;
	}

	return FEditorViewportClient::UpdateMouseDelta();
}

//! 
void FNaMapPartsEditorViewportClient::MouseMove( FViewport* InViewport, int32 x, int32 y )
{
	FEditorViewportClient::MouseMove( InViewport, x, y );

	FVector	pos, dir;

	CalcWorldPosition( InViewport, x, y, pos, dir );
	m_owner.Pin()->OnHoverMouse( pos, dir );
}

//! 
void FNaMapPartsEditorViewportClient::CapturedMouseMove( FViewport* InViewport, int32 InMouseX, int32 InMouseY )
{
	FEditorViewportClient::CapturedMouseMove( InViewport, InMouseX, InMouseY );

/*	if ( m_bDrawing ){
		FVector	pos, dir;

		CalcWorldPosition( InViewport, InMouseX, InMouseY, pos, dir );
		m_owner.Pin()->OnHoverMouse( pos, dir );
		m_owner.Pin()->OnClickViewport( pos, dir );

		//DrawDebugSphere( GetWorld(), ClickOrigin + ViewDir * 100.0f, 10.0f, 16, FColor::Red, false, 3.0f );
	}*/
}

//! 	
void FNaMapPartsEditorViewportClient::ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY)
{
	if ( Event == EInputEvent::IE_Released ){
		FVector2D	ScreenPos(HitX, HitY);
		FVector		ClickOrigin, ViewDir;

		View.DeprojectFVector2D( ScreenPos, ClickOrigin, ViewDir );

		m_owner.Pin()->OnClickViewport( ClickOrigin, ViewDir, Key );
	}
}

//! 
void FNaMapPartsEditorViewportClient::CalcWorldPosition( FViewport* viewport, int32 x, int32 y, FVector& outVal, FVector& outDir )
{
	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
		viewport,
		GetScene(),
		EngineShowFlags )
		.SetRealtimeUpdate( IsRealtime() )
	);

	FSceneView*	View = CalcSceneView( &ViewFamily );
	FVector2D	ScreenPos( x, y );
	FVector		ClickOrigin, ViewDir;

	View->DeprojectFVector2D( ScreenPos, outVal, outDir );
}
