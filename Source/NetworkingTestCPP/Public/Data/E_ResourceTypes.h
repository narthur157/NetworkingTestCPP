// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "E_ResourceTypes.generated.h"

UENUM(BlueprintType)
enum EResourceTypes
{
	Mana  UMETA(DisplayName = "Mana"),
	Energy     UMETA(DisplayName = "Energy"),
	Rage  UMETA(DisplayName = "Rage")

};