// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "S_SpellProjectileInfo.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FSpellSubActorProjectileParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UParticleSystem* Emitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UParticleSystem* TargetingEmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UMaterial* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellParameters")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellParameters")
		float Healing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellParameters")
		float ScalingMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileParameters")
		float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileParameters")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileParameters")
		TArray< TSubclassOf <AActor> > ValidTargetClasses;

};


USTRUCT(BlueprintType)
struct FSpellProjectileStageParameters
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UParticleSystem* EmitterOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UParticleSystem* TargetingEmitterOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UStaticMesh* MeshOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UMaterial* MaterialOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellParameters")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellParameters")
		float Healing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellParameters")
		float ScalingMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileParameters")
		float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileParameters")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileParameters")
		TArray< TSubclassOf <AActor> > ValidTargetClasses;




};


USTRUCT(BlueprintType)
struct FSpellProjectileBaseParameters
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UParticleSystem* Emitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UParticleSystem* TargetingEmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UMaterial* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
		TArray<FSpellProjectileStageParameters> SpellStageInfo;

};

