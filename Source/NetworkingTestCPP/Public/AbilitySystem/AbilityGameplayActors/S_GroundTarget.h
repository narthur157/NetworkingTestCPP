// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability_Master.h"
#include "S_GroundTarget.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FTargetedGroundInfo
{

	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetedAOEInfo")
		float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetedAOEInfo")
		FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetedAOEInfo")
		AAbility_Master * SpellToCast;


};