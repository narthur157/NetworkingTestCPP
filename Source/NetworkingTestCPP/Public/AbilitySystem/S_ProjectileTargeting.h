// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S_ProjectileTargeting.generated.h"
 /**
 *
 */
USTRUCT(BlueprintType)
struct FProjectileTargetingInfo
{

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetedAOEInfo")
		float Range;


};