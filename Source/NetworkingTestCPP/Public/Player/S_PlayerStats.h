// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S_PlayerStats.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum EPlayerMovementState
{
	Rooted		UMETA(DisplayName = "Rooted"),
	Slowed		UMETA(DisplayName = "Slowed"),
	Stunned		UMETA(DisplayName = "Stunned"),
	Free		UMETA(DisplayName = "Free")
};

UENUM(BlueprintType)
enum EPlayerActionState
{
	Idle		UMETA(DisplayName = "Idle"),
	Targeting	UMETA(DisplayName = "Targeting"),
	Casting		UMETA(DisplayName = "Casting")
	
};

USTRUCT(BlueprintType)
struct FPlayerBaseStats
{

	GENERATED_USTRUCT_BODY()

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float Mana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float MaxMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float Energy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float MaxEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float Rage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float MaxRage;
	//0 = free, 1 = fullcost 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float AbilityCostReduction = 1.0;
	//0 = instant, 1 = fullcast 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float CastTimeReduction = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		TEnumAsByte<EPlayerMovementState> PlayerMovementState = Free;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		TEnumAsByte<EPlayerActionState> PlayerActionState = Idle;

	





};