// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class NaNRPGEditorTarget : TargetRules
{
	public NaNRPGEditorTarget(TargetInfo Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange(new string[] { "NaNRPG", "NaNRPGEd" });
	}
}
