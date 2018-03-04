// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EditorStyleSet.h"

/**
 * 
 */
class FNaMapPartsEditorCommands : public TCommands<FNaMapPartsEditorCommands>
{

public:
	FNaMapPartsEditorCommands() : TCommands<FNaMapPartsEditorCommands>
	(
		"NaMapPartsEditor", // Context name for fast lookup
		NSLOCTEXT("Contexts", "NaMapPartsEditor", "NaMapParts Editor"), // Localized context name for displaying
		"EditorViewport",  // Parent
		FEditorStyle::GetStyleSetName() // Icon Style Set
	)
	{
	}
	
	/**
	 * NaMapPartsEditor Commands
	 */

	//! �e�B�A�[�ҏW���[�h
	TSharedPtr< FUICommandInfo >	SelectTierTool;
	//! �}�C�N���ҏW���[�h
	TSharedPtr< FUICommandInfo >	SelectMinecraftTool;

	//! �f�ʕ\��
	TSharedPtr< FUICommandInfo >	SetCrossSection;
	//! 

	//! �y��
	TSharedPtr< FUICommandInfo >	SelectDrawPenTool;
	//! �{�b�N�X
	TSharedPtr< FUICommandInfo >	SelectDrawBoxTool;
	//! �t�B��
	TSharedPtr< FUICommandInfo >	SelectDrawFillTool;
	//! �I��
	TSharedPtr< FUICommandInfo >	SelectSelectionTool;
	
	/**  */
	TSharedPtr< FUICommandInfo > SetShowWireframe;
	TSharedPtr< FUICommandInfo > SetShowVertexColor;
	TSharedPtr< FUICommandInfo > SetDrawUVs;
	TSharedPtr< FUICommandInfo > SetShowGrid;
	TSharedPtr< FUICommandInfo > SetShowBounds;
	TSharedPtr< FUICommandInfo > SetShowCollision;
	TSharedPtr< FUICommandInfo > ResetCamera;
	TSharedPtr< FUICommandInfo > SetShowSockets;
	TSharedPtr< FUICommandInfo > SetDrawAdditionalData;

	// View Menu Commands
	TSharedPtr< FUICommandInfo > SetShowNormals;
	TSharedPtr< FUICommandInfo > SetShowTangents;
	TSharedPtr< FUICommandInfo > SetShowBinormals;
	TSharedPtr< FUICommandInfo > SetShowPivot;
	TSharedPtr< FUICommandInfo > SetShowVertices;

	// Collision Menu Commands
	TSharedPtr< FUICommandInfo > CreateDOP10X;
	TSharedPtr< FUICommandInfo > CreateDOP10Y;
	TSharedPtr< FUICommandInfo > CreateDOP10Z;
	TSharedPtr< FUICommandInfo > CreateDOP18;
	TSharedPtr< FUICommandInfo > CreateDOP26;
	TSharedPtr< FUICommandInfo > CreateBoxCollision;
	TSharedPtr< FUICommandInfo > CreateSphereCollision;
	TSharedPtr< FUICommandInfo > CreateSphylCollision;
	TSharedPtr< FUICommandInfo > CreateAutoConvexCollision;
	TSharedPtr< FUICommandInfo > RemoveCollision;
	TSharedPtr< FUICommandInfo > ConvertBoxesToConvex;
	TSharedPtr< FUICommandInfo > CopyCollisionFromSelectedMesh;

	// Mesh Menu Commands
	TSharedPtr< FUICommandInfo > FindSource;

	TSharedPtr< FUICommandInfo > ChangeMesh;

	TSharedPtr< FUICommandInfo > SaveGeneratedLODs;

	/**
	 * Initialize commands
	 */
	virtual void RegisterCommands() override;

public:
};
