// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"

//Logging 
DECLARE_LOG_CATEGORY_EXTERN(AbilitySystemInitialization, Verbose, All);
DECLARE_LOG_CATEGORY_EXTERN(AbilitySystemRun, Verbose, All);
DECLARE_LOG_CATEGORY_EXTERN(SpellCasting, Verbose, All);

#define NETMODE_WORLD (((GEngine == nullptr) || (GetWorld() == nullptr)) ? TEXT("") \
        : (GEngine->GetNetMode(GetWorld()) == NM_Client) ? TEXT("[Client] ") \
        : (GEngine->GetNetMode(GetWorld()) == NM_ListenServer) ? TEXT("[ListenServer] ") \
        : (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer) ? TEXT("[DedicatedServer] ") \
        : TEXT("[Standalone] "))

