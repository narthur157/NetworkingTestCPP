// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystem/TargetTypes.h"
#include "AbilitySystem/SpellBookComponent.h"
#include "AbilitySystem/AbilityTargeting.h"
#include "WaitForTarget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitTargetDataOutputPin, const FTargetDataHandle&, Data);


UCLASS()
class NETWORKINGTESTCPP_API UWaitForTarget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	UWaitForTarget(const FObjectInitializer& FObjectInitializer);
	
public:
	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataOutputPin AbilityConfirmed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Flow Control")
	static UWaitForTarget* WaitForTarget(const UObject* WorldContextObject, AAbility_Master* targetingAbility, TSubclassOf <AAbilityTargeting> TargetingClass);

	virtual void Activate() override;

	UFUNCTION()
	void ExecuteOnTargetDataRecieved(const FTargetDataHandle &Data);



protected:
	void SpawnTargetingActor();


private:

	bool Active;
	AAbility_Master * targetingAbility;
	USpellBookComponent* owningSpellbook;
	TSubclassOf <AAbilityTargeting> targetingClass;
	
};
