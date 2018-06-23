// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AbilityTasks/WaitForTarget.h"
#include "Engine/World.h"



UWaitForTarget::UWaitForTarget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

UWaitForTarget* UWaitForTarget::WaitForTarget(const UObject* WorldContextObject, AAbility_Master* targetingAbility, TSubclassOf<AAbilityTargeting> TargetingClass)
{
	UWaitForTarget* BlueprintNode = NewObject<UWaitForTarget>();
	BlueprintNode->targetingAbility = targetingAbility;
	BlueprintNode->owningSpellbook = targetingAbility->getSpellBook();
	BlueprintNode->targetingClass = TargetingClass;

	return BlueprintNode;

}


// on node encountered
void UWaitForTarget::Activate()
{
	owningSpellbook->TargetDispatcher.AddDynamic(this, &UWaitForTarget::ExecuteOnTargetDataRecieved);

	if (owningSpellbook->GetOwnerRole() != ROLE_Authority)
	{
		

		if (targetingClass == nullptr)
		{
			return;
		}
		else
		{
			SpawnTargetingActor();
		}

	}

	owningSpellbook->TargetingBegin.Broadcast();
	targetingAbility->PlayAnimation(targetingAbility->getTargetingAnimation(), 1.0);

}


//exec pin
void UWaitForTarget::ExecuteOnTargetDataRecieved(const FTargetDataHandle &Data)
{


	owningSpellbook->TargetingConfirmed.Broadcast();
	targetingAbility->StopAnimation(targetingAbility->getTargetingAnimation());
	AbilityConfirmed.Broadcast(Data);

}

void UWaitForTarget::SpawnTargetingActor()
{

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = owningSpellbook->GetOwner();
	spawnParams.Instigator = targetingAbility->Instigator;
	//TODO make this a template, to accept different inputs
	AAbilityTargeting* newTargetingActor = owningSpellbook->GetOwner()->GetWorld()->SpawnActor<AAbilityTargeting>(targetingClass, spawnParams);

	// register targeting actor in delegate chain
	newTargetingActor->SendData.AddDynamic(owningSpellbook, &USpellBookComponent::ValidateAndBroadcastValidData);
	owningSpellbook->ConfirmAbility.AddDynamic(newTargetingActor, &AAbilityTargeting::ConfirmTargeting);

	newTargetingActor->StartTargeting(targetingAbility);
	
	

}