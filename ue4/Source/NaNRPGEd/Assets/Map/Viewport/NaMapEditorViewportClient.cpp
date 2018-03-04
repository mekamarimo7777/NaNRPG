// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPGEd.h"
#include "NaMapEditorViewportClient.h"

#include "SNaMapEditorViewport.h"

//!
FNaMapEditorViewportClient::FNaMapEditorViewportClient( const TSharedRef<SNaMapEditorViewport>& InEditorViewport, FAdvancedPreviewScene& InPreviewScene )
: FEditorViewportClient( nullptr, &InPreviewScene, StaticCastSharedRef<SEditorViewport>( InEditorViewport ) )
, m_owner( InEditorViewport )
, m_bDrawing( false )
{
	SetCameraSpeedSetting(2);
}

FNaMapEditorViewportClient::~FNaMapEditorViewportClient()
{
}

//! 
void FNaMapEditorViewportClient::Tick( float DeltaSeconds )
{
	FEditorViewportClient::Tick( DeltaSeconds );

	// Tick the preview scene world.
	if ( !GIntraFrameDebuggingGameThread ){
		PreviewScene->GetWorld()->Tick( LEVELTICK_All, DeltaSeconds );
	}
}

//! 
bool FNaMapEditorViewportClient::InputKey(FViewport* InViewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad )
{
	bool	bHandled = false;

	return bHandled ? true : FEditorViewportClient::InputKey( InViewport, ControllerId, Key, Event, AmountDepressed, bGamepad );
}

//! 
void FNaMapEditorViewportClient::UpdateMouseDelta()
{
	if ( m_bDrawing ){
		return;
	}

	return FEditorViewportClient::UpdateMouseDelta();
}

//! 
void FNaMapEditorViewportClient::MouseMove( FViewport* InViewport, int32 x, int32 y )
{
	FEditorViewportClient::MouseMove( InViewport, x, y );
}

//! 
void FNaMapEditorViewportClient::CapturedMouseMove( FViewport* InViewport, int32 InMouseX, int32 InMouseY )
{
	FEditorViewportClient::CapturedMouseMove( InViewport, InMouseX, InMouseY );
}

//! 	
void FNaMapEditorViewportClient::ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY)
{
	if ( Event == EInputEvent::IE_Released ){
		FVector2D	ScreenPos(HitX, HitY);
		FVector		ClickOrigin, ViewDir;

		View.DeprojectFVector2D( ScreenPos, ClickOrigin, ViewDir );

		m_owner.Pin()->OnClickViewport( ClickOrigin, ViewDir, Key );
	}
}

//! 
void FNaMapEditorViewportClient::CalcWorldPosition( FViewport* viewport, int32 x, int32 y, FVector& outVal, FVector& outDir )
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
