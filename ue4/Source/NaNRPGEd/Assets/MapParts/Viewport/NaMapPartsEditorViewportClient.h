// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EditorViewportClient.h"

class SNaMapPartsEditorViewport;
class FAdvancedPreviewScene;

/** Viewport Client for the preview viewport */
class FNaMapPartsEditorViewportClient : public FEditorViewportClient, public TSharedFromThis<FNaMapPartsEditorViewportClient>
{

public:
	FNaMapPartsEditorViewportClient( const TSharedRef<SNaMapPartsEditorViewport>& InEditorViewport, FAdvancedPreviewScene& InPreviewScene );
	~FNaMapPartsEditorViewportClient();
	
protected:
	//! 
	virtual void	Tick( float DeltaSeconds ) override;

	//! 
	virtual bool	InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed = 1.f, bool bGamepad=false) override;

	//! 
	virtual void	UpdateMouseDelta() override;
	//! 
	virtual void	MouseMove( FViewport* InViewport, int32 x, int32 y ) override;
	//! 
	virtual void	CapturedMouseMove( FViewport* InViewport, int32 InMouseX, int32 InMouseY ) override;
	//! 	
	virtual void	ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY) override;

	//! 
	void	CalcWorldPosition( FViewport* viewport, int32 x, int32 y, FVector& outVal, FVector& outDir );

public:
	//! 
	TWeakPtr<class SNaMapPartsEditorViewport>	m_owner;
	//! 
	bool	m_bDrawing;
};
