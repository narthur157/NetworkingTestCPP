// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Engine/World.h"
#include "ExtendableTimer.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKINGTESTCPP_API UExtendableTimer : public UObject
{
	GENERATED_BODY()


public:

	UExtendableTimer();

	UFUNCTION(BlueprintCallable)
	void Resume();
		
	// should probably be able to get world from object
	void InitTimer(UWorld* World, UObject* Object, FName UpdateFunction, FName OnEndFunction, float Duration, float UpdateRate);
	

	//void End();

	void Pause();

	void ExtendTimerDuration(float SecondsToExtend);
	void ReduceTimerDuration(float SecondsToReduce);

	UFUNCTION()
	void Update();
	UFUNCTION()
	void Finished();

	float GetTimeRemaining();
	float GetTotalTimeElapsed();
	float GetDuration();

	
private:



	UWorld * ContextWorld;

	FTimerHandle UpdateTimerHandle;
	FTimerHandle EndTimerHandle;
	FTimerDynamicDelegate UpdateDel;
	FTimerDynamicDelegate EndDel;

	/** Functions that were passed in to be called on the update/end timers **/
	UFunction* ObjectUpdateFunction;
	UFunction* ObjectEndFunction;
	UObject* CallbackObject;

	/**	Time of older timers **/
	float totalTimeElapsed = 0;
	/** Total duration of the timer, this value could change often (as we add/subtract time to the total time)**/
	float totalDuration;

	/** When the timer started **/
	float startTime;

	
};
