// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"

//Logging for a that troublesome system
DECLARE_LOG_CATEGORY_EXTERN(AbilitySystemInitialization, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(AbilitySystemRun, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(SpellCasting, Log, All);

#define NETMODE_WORLD (((GEngine == nullptr) || (GetWorld() == nullptr)) ? TEXT("") \
        : (GEngine->GetNetMode(GetWorld()) == NM_Client) ? TEXT("[Client] ") \
        : (GEngine->GetNetMode(GetWorld()) == NM_ListenServer) ? TEXT("[ListenServer] ") \
        : (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer) ? TEXT("[DedicatedServer] ") \
        : TEXT("[Standalone] "))

enum ELogCategory
{
	LogInitialization,
	LogRun

};