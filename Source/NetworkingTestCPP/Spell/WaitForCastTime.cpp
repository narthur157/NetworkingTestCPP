// Fill out your copyright notice in the Description page of Project Settings.

#include "WaitForCastTime.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Spell/SpellBookComponent.h"
#include "Engine/World.h"


#define CASTTICKRATE .01

UWaitForCastTime* UWaitForCastTime::WaitForCastTime(const UObject* WorldContextObject, AAbility_Master* CastingAbility, const float OverrideDuration, bool bUseOverrideDuration)
{
	UWaitForCastTime* Node = NewObject<UWaitForCastTime>();
	Node->WorldContextObject = WorldContextObject;
	Node->CastingAbility = CastingAbility;
	Node->AbilityTimer = CastingAbility->getTimelineComponent();
	bUseOverrideDuration ? Node->Duration = OverrideDuration : Node->Duration = CastingAbility->getCurrentStage().castTime;
	
	return Node;
}

void UWaitForCastTime::Activate()
{
	if (nullptr == WorldContextObject)
	{
		FFrame::KismetExecutionMessage(TEXT("Invalid WorldContextObject. Cannot execute CastTimer."), ELogVerbosity::Error);
		return;
	}
	if (Active)
	{
		FFrame::KismetExecutionMessage(TEXT("CastTimer is already running."), ELogVerbosity::Warning);
		return;
	}
	if (Duration <= 0.0f)
	{
		FFrame::KismetExecutionMessage(TEXT("CastTimer TimerDuration cannot be less or equal to 0."), ELogVerbosity::Warning);
		return;
	}
	if (CastingAbility->getTimelineComponent() != nullptr)
	{
		FFrame::KismetExecutionMessage(TEXT("Ability is already using a timeline"), ELogVerbosity::Warning);
		return;
	}
	
	Active = true;

	AbilityTimer = NewObject<UExtendableTimer>();
	CastingAbility->setTimelineComponent(AbilityTimer);
	AbilityTimer->InitTimer(WorldContextObject->GetWorld(), this, FName{ TEXT("_Update") }, FName{ TEXT("_Finish") }, Duration, CASTTICKRATE);
	CastingAbility->getSpellBook()->CastBegun.Broadcast(CastingAbility);


}

void UWaitForCastTime::_Update()
{
	
	CastingAbility->getSpellBook()->CastingBarUpdate.Broadcast(AbilityTimer->GetTotalTimeElapsed(), AbilityTimer->GetDuration());
	
}

void UWaitForCastTime::_Finish()
{

	
	CastingAbility->getSpellBook()->CastCompleted.Broadcast(CastingAbility);
	CastingAbility->StopAnimation(CastingAbility->getCastingAnimation());
	Finished.Broadcast();

	CastingAbility->setTimelineComponent(nullptr);
	AbilityTimer = nullptr;
	Active = false;
}