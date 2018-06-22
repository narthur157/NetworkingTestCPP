// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S_SpellAOEInfo.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum EAOEType
{
	OneAndDone			UMETA(DisplayName = "OneAndDone"),
	Duration			UMETA(DisplayName = "Duration"),
	OwnerControlled		UMETA(DisplayName = "OwnerController")

};

USTRUCT(BlueprintType)
struct FSpellAOEStageParameters
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UMaterial* SpellDecalOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UParticleSystem* EmitterOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UParticleSystem* TargetingEmitterOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UStaticMesh* MeshOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		TArray<UMaterialInstance*> MeshMaterialsOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Healing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Duration;


	// -1 = infinite
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxNumberTargetsToHit = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float ScalingMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TEnumAsByte<EAOEType> AOEType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray< TSubclassOf <AActor> > ValidTargetClasses;
	

};


USTRUCT(BlueprintType)
struct FSpellAOEBaseParameters
{

	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UMaterial* SpellDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UMaterial* TargetingSpellDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UParticleSystem* Emitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UParticleSystem* TargetingEmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		TArray<UMaterialInstance*> MeshMaterials;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<AActor*> SpawnedInstances;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
		TArray<FSpellAOEStageParameters> SpellStageInfo;


};


USTRUCT(BlueprintType)
struct FSpellSubActorAOEParameters
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UMaterial* SpellDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UMaterial* TargetingSpellDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UParticleSystem* Emitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UParticleSystem* TargetingEmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		TArray<UMaterialInstance*> MeshMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Healing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Duration;

	// -1 = infinite
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxNumberTargetsToHit = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float ScalingMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TEnumAsByte<EAOEType> AOEType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray< TSubclassOf <AActor> > ValidTargetClasses;

};

