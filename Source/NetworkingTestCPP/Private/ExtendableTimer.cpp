// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtendableTimer.h"




UExtendableTimer::UExtendableTimer()
{


}

void UExtendableTimer::InitTimer(UWorld* World, UObject* Object, FName UpdateFunction, FName OnEndFunction, float Duration, float UpdateRate)
{

	ContextWorld = World;

	startTime = ContextWorld->GetTimeSeconds();
	totalDuration = Duration;

	CallbackObject = Object;
	ObjectUpdateFunction = Object->FindFunction(UpdateFunction);
	ObjectEndFunction = Object->FindFunction(OnEndFunction);

	if (ObjectUpdateFunction)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("UpdateFunction"));
		
	}

	if (ObjectEndFunction)
	{

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("EndFunction"));
	}


	if (ObjectUpdateFunction && ObjectUpdateFunction->NumParms > 0)
	{
		return;
	}

	if (ObjectEndFunction && ObjectEndFunction->NumParms > 0)
	{
		return;
	}


	EndDel.BindUFunction(this, FName{ TEXT("Finished") });
	ContextWorld->GetTimerManager().SetTimer(EndTimerHandle, EndDel, Duration, false);
	if (ObjectUpdateFunction)
	{
		UpdateDel.BindUFunction(this, FName{ TEXT("Update") });
		ContextWorld->GetTimerManager().SetTimer(UpdateTimerHandle, UpdateDel, UpdateRate, true);
	}

}

void UExtendableTimer::Resume()
{
	ContextWorld->GetTimerManager().UnPauseTimer(EndTimerHandle);
	ContextWorld->GetTimerManager().UnPauseTimer(UpdateTimerHandle);

}



void UExtendableTimer::Pause()
{

	ContextWorld->GetTimerManager().PauseTimer(EndTimerHandle);
	ContextWorld->GetTimerManager().PauseTimer(UpdateTimerHandle);
	
}


void UExtendableTimer::Update()
{

	CallbackObject->ProcessEvent(ObjectUpdateFunction, NULL);

}

void UExtendableTimer::Finished()
{

	CallbackObject->ProcessEvent(ObjectEndFunction, NULL);
	ContextWorld->GetTimerManager().ClearTimer(UpdateTimerHandle);

}


void UExtendableTimer::ExtendTimerDuration(float SecondsToExtend)
{
	
	totalTimeElapsed += ContextWorld->GetTimerManager().GetTimerElapsed(EndTimerHandle);
	totalDuration += SecondsToExtend;
	ContextWorld->GetTimerManager().SetTimer(EndTimerHandle, EndDel, totalDuration - totalTimeElapsed, false);
	

}

void UExtendableTimer::ReduceTimerDuration(float SecondsToReduce)
{
	totalTimeElapsed += ContextWorld->GetTimerManager().GetTimerElapsed(EndTimerHandle);
	if (totalDuration - SecondsToReduce <= totalTimeElapsed)
	{
		ContextWorld->GetTimerManager().ClearTimer(EndTimerHandle);
		Finished();

	}
	else
	{

		totalDuration -= SecondsToReduce;
		ContextWorld->GetTimerManager().SetTimer(EndTimerHandle, EndDel, totalDuration - totalTimeElapsed, false);

	}

}

float UExtendableTimer::GetTimeRemaining()
{
	return ContextWorld->GetTimerManager().GetTimerRemaining(EndTimerHandle);
}

float UExtendableTimer::GetTotalTimeElapsed()
{
	// Time of older timers (we may have been extended) + time of current timer
	return totalTimeElapsed + ContextWorld->GetTimerManager().GetTimerElapsed(EndTimerHandle);

}

float UExtendableTimer::GetDuration()
{
	return totalDuration;

}