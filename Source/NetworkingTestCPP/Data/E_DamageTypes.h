// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "E_DamageTypes.generated.h"

UENUM(BlueprintType)
enum DamageTypes
{
	Physical  UMETA(DisplayName= "Physical"),
	Magic     UMETA(DisplayName = "Magic"),
	Absolute  UMETA(DisplayName = "Absolute")

};