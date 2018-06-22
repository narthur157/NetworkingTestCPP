// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RPGTemplateGameInstance.generated.h"

/**
 * 
 */


UCLASS()
class NETWORKINGTESTCPP_API URPGTemplateGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	URPGTemplateGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "SpellGlobalVarsHelper")
		UMaterial* getDefaultTargetingDecalAOE();

	UFUNCTION(BlueprintCallable, Category = "SpellGlobalVarsHelper")
		UMaterial* getDefaultDecalAOE();

private:
	UPROPERTY(VisibleAnywhere, Category = "SpellGlobalVars")
		UMaterial* DefaultSpellTargetingDecalAOE;

	UPROPERTY(VisibleAnywhere, Category = "SpellGlobalVars")
		UMaterial* DefaultSpellDecalAOE;
	


};
