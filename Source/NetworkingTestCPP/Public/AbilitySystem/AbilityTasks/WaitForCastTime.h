// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystem/Ability_Master.h"
#include "Engine/TimelineTemplate.h"
#include "ExtendableTimer.h"
#include "WaitForCastTime.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCastingTimerOutputPin);


UCLASS()
class NETWORKINGTESTCPP_API UWaitForCastTime : public UBlueprintAsyncActionBase
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
		FCastingTimerOutputPin Finished;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Timer|Mini-Timer")
		static UWaitForCastTime* WaitForCastTime(const UObject* WorldContextObject, AAbility_Master* CastingAbility, const float OverrideDuration, bool bUseOverrideDuration);

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	//~UBlueprintAsyncActionBase interface

	UFUNCTION()
		void _Update();
	UFUNCTION()
		void _Finish();

private:
	
	
private:
	const UObject* WorldContextObject;
	bool Active;
	FTimerHandle Timer;
	AAbility_Master * CastingAbility;

	FOnTimelineEvent onTimelineCallback;
	FOnTimelineEventStatic onTimelineEnd;

	UPROPERTY()
	TEnumAsByte<ETimelineDirection::Type> TimelineDirection = ETimelineDirection::Forward;

	UExtendableTimer * AbilityTimer;
	float Duration;

	
	
	
};
