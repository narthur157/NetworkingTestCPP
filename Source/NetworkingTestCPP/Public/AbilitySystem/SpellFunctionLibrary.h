// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilityGameplayActors/S_SpellProjectileInfo.h"
#include "AbilityGameplayActors/S_SpellAOEInfo.h"
#include "AbilitySystem/TargetTypes.h"
#include "NetworkingTestCPP.h"
#include "SpellFunctionLibrary.generated.h"

UCLASS()
class NETWORKINGTESTCPP_API USpellFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
		
		////////////////////////////////////////////////////////////////
		// Conversion from Subactor structs to spawning structs
		// This can probably be done in a cleaner way

		UFUNCTION(BlueprintPure, meta = (DisplayName = "SpellProjectileBaseParameters To SpellSubActorProjectileParameters", CompactNodeTitle = "->", BlueprintAutocast), Category = "SubActorGeneration|Projectile")
		static FSpellSubActorProjectileParameters Conv_SpellProjectileBaseParametersToSpellSubActorProjectileParameters(FSpellProjectileBaseParameters ProjectileBaseParameters, int32 Rank);

		UFUNCTION(BlueprintPure, meta = (DisplayName = "SpellAOEBaseParameters To SpellSubActorAOEParameters", CompactNodeTitle = "->", BlueprintAutocast), Category = "SubActorGeneration|AOE")
		static FSpellSubActorAOEParameters Conv_SpellAOEBaseParametersToSpellSubActorAOEParameters(FSpellAOEBaseParameters AOEBaseParameters, int32 Rank);

		////////////////////////////////////////////////////////////////
		// Conversion from different target data types to target data handles, structure polymorphism is not supported by blueprints
		// This can probably be done in a cleaner way

		UFUNCTION(BlueprintPure, meta = (DisplayName = "TargetData To TargetDataHandle", CompactNodeTitle = "->", BlueprintAutocast), Category = "TargetHandleGeneration")
		static FTargetDataHandle Conv_TargetDataToTargetDataHandle(FTargetData Data);

		UFUNCTION(BlueprintPure, meta = (DisplayName = "TargetData_Actor To TargetDataHandle", CompactNodeTitle = "->", BlueprintAutocast), Category = "TargetHandleGeneration")
		static FTargetDataHandle Conv_TargetData_ActorToTargetDataHandle(FTargetData_Actor Data)
		{
			FTargetDataHandle newData(&Data);
			return newData;
		}

		UFUNCTION(BlueprintPure, meta = (DisplayName = "TargetData_Projectile To TargetDataHandle", CompactNodeTitle = "->", BlueprintAutocast), Category = "TargetHandleGeneration")
		static FTargetDataHandle Conv_TargetData_ProjectileToTargetDataHandle(FTargetData_Projectile Data)
		{

			FTargetDataHandle newData(&Data);
			return newData;

		}

		////////////////////////////////////////////////////////////////
		// Target Data Handle Operations
		// Cannot use structure methods in blueprint, workaround

		UFUNCTION(BlueprintPure, Category = "Targeting Data")
		static bool HasValidData(FTargetDataHandle Data)
		{
			if (Data.Data.Num() > 0)
			{
				return true;
			}
			else
			{
				return false;
			}

		}


		UFUNCTION(BlueprintPure, Category = "Targeting Data")
		static bool HasTargetedActors(FTargetDataHandle Data)
		{
			return Data.getData()->HasTargetedActors();

		}

		UFUNCTION(BlueprintPure, Category = "Targeting Data")
		static bool HasSourceLocation(FTargetDataHandle Data)
		{
			
			return Data.getData()->HasSourceLocation();
		}

		UFUNCTION(BlueprintPure, Category = "Targeting Data")
		static bool HasTargetLocation(FTargetDataHandle Data)
		{
			return Data.getData()->HasTargetLocation();

		}

		UFUNCTION(BlueprintPure, Category = "Targeting Data")
		static AActor* getTargetedActor(FTargetDataHandle Data)
		{
			return Data.getData()->getTargetedActor();

		}

		UFUNCTION(BlueprintPure, Category = "Targeting Data")
		static TArray<AActor*> getTargetedActors(FTargetDataHandle Data)
		{
			return Data.getData()->getTargetedActors();

		}

		UFUNCTION(BlueprintPure, Category = "Targeting Data")
		static FTransform getSourceLocation(FTargetDataHandle Data)
		{
			return Data.getData()->getSourceLocation();

		}

		UFUNCTION(BlueprintPure, Category = "Targeting Data")
		static FTransform getTargetLocation(FTargetDataHandle Data)
		{
			return Data.getData()->getTargetLocation();

		}

		UFUNCTION(BlueprintPure, Category = "Targeting Data")
		static float getChargeTime(FTargetDataHandle Data)
		{

			return Data.getData()->getChargeTime();
		}

	
};
