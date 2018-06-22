// Fill out your copyright notice in the Description page of Project Settings.

#include "SpellFunctionLibrary.h"
#include "NetworkingTestCPPGameMode.h"


FSpellSubActorProjectileParameters USpellFunctionLibrary::Conv_SpellProjectileBaseParametersToSpellSubActorProjectileParameters(FSpellProjectileBaseParameters ProjectileBaseParameters, int32 Rank)
{

	FSpellSubActorProjectileParameters newProjectileParameters;
	FSpellProjectileStageParameters currentStageParams = ProjectileBaseParameters.SpellStageInfo[Rank];
	

	currentStageParams.EmitterOverride != nullptr ? newProjectileParameters.Emitter = currentStageParams.EmitterOverride
													: newProjectileParameters.Emitter = ProjectileBaseParameters.Emitter;


	currentStageParams.MeshOverride != nullptr ? newProjectileParameters.Mesh = currentStageParams.MeshOverride
												: newProjectileParameters.Mesh = ProjectileBaseParameters.Mesh;


	currentStageParams.TargetingEmitterOverride != nullptr ? newProjectileParameters.TargetingEmitter = currentStageParams.TargetingEmitterOverride
															: newProjectileParameters.TargetingEmitter = ProjectileBaseParameters.TargetingEmitter;


	currentStageParams.MaterialOverride != nullptr ? newProjectileParameters.Material = currentStageParams.MaterialOverride
													: newProjectileParameters.Material = ProjectileBaseParameters.Material;

	newProjectileParameters.Damage = ProjectileBaseParameters.SpellStageInfo[Rank].Damage;
	newProjectileParameters.Healing = ProjectileBaseParameters.SpellStageInfo[Rank].Healing;
	newProjectileParameters.ScalingMultiplier = ProjectileBaseParameters.SpellStageInfo[Rank].ScalingMultiplier;
	newProjectileParameters.Speed = ProjectileBaseParameters.SpellStageInfo[Rank].Speed;
	newProjectileParameters.Range = ProjectileBaseParameters.SpellStageInfo[Rank].Range;
	newProjectileParameters.ValidTargetClasses = ProjectileBaseParameters.SpellStageInfo[Rank].ValidTargetClasses;

	return newProjectileParameters;

}



FSpellSubActorAOEParameters USpellFunctionLibrary::Conv_SpellAOEBaseParametersToSpellSubActorAOEParameters(FSpellAOEBaseParameters AOEBaseParameters, int32 Rank)
{

	FSpellSubActorAOEParameters newAOEParameters;
	FSpellAOEStageParameters currentStageParams = AOEBaseParameters.SpellStageInfo[Rank];

	currentStageParams.SpellDecalOverride != nullptr ? newAOEParameters.SpellDecal = currentStageParams.SpellDecalOverride
		: newAOEParameters.SpellDecal = AOEBaseParameters.SpellDecal;


	currentStageParams.EmitterOverride != nullptr ? newAOEParameters.Emitter = currentStageParams.EmitterOverride
													: newAOEParameters.Emitter = AOEBaseParameters.Emitter;


	currentStageParams.TargetingEmitterOverride != nullptr ? newAOEParameters.TargetingEmitter = currentStageParams.TargetingEmitterOverride
		: newAOEParameters.TargetingEmitter = AOEBaseParameters.TargetingEmitter;


	if (currentStageParams.MeshOverride != nullptr)
	{
		newAOEParameters.Mesh = currentStageParams.MeshOverride;
		newAOEParameters.MeshMaterials = currentStageParams.MeshMaterialsOverride;
	}

	else
	{

		newAOEParameters.Mesh = AOEBaseParameters.Mesh;
		newAOEParameters.MeshMaterials = AOEBaseParameters.MeshMaterials;

	}

	newAOEParameters.TargetingSpellDecal = AOEBaseParameters.TargetingSpellDecal;
	newAOEParameters.AOEType = currentStageParams.AOEType;
	newAOEParameters.Damage = currentStageParams.Damage;
	newAOEParameters.Healing = currentStageParams.Healing;
	newAOEParameters.ScalingMultiplier = currentStageParams.ScalingMultiplier;
	newAOEParameters.Radius = currentStageParams.Radius;
	newAOEParameters.Duration = currentStageParams.Duration;
	newAOEParameters.ValidTargetClasses = currentStageParams.ValidTargetClasses;

	return newAOEParameters;

}


FTargetDataHandle USpellFunctionLibrary::Conv_TargetDataToTargetDataHandle(FTargetData Data)
{
	FTargetDataHandle newData;
	newData.makeData(&Data);
	return newData;
}