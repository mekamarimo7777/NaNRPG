// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class NaNRPGTarget : TargetRules
{
	public NaNRPGTarget(TargetInfo Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange(new string[] { "NaNRPG" });
	}
}
