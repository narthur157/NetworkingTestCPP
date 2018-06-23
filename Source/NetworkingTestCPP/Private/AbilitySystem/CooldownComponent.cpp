// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/CooldownComponent.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UCooldownComponent::UCooldownComponent()
{

}

// init params needed for object to function
void UCooldownComponent::setParams(UWorld* newWorld)
{
	world = newWorld;


}

void UCooldownComponent::placeTagOnCooldown(FName ElementToAdd, float Duration)
{
	float startTime = world->GetTimeSeconds();
	float endTime = startTime + Duration;

	// add to the cooldown array for easy removal when the endtime comes
	addToCooldownArray(endTime, ElementToAdd);
	addToCooldownMap(startTime, endTime, ElementToAdd);

	// start timer, if timer is not already running
	if (world->GetTimerManager().IsTimerActive(cooldownTimer) != true)
	{

		FTimerDynamicDelegate buffDebuffTimer;
		buffDebuffTimer.BindDynamic(this, &UCooldownComponent::pingBuffDebuffArray);
		world->GetTimerManager().SetTimer(cooldownTimer, buffDebuffTimer, .01, true, 0);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("timer started"));

	}

}

void UCooldownComponent::removeFromCooldown(FName ElementToRemove)
{

	int32 indexToRemove = getFirstIndexOfElementName(ElementToRemove);
	FAbilityCooldownContainer *mapElementToRemove = cooldownMap.Find(ElementToRemove);

	if (mapElementToRemove->count() == 1)
	{

		cooldownMap.Remove(ElementToRemove);
		UE_LOG(SpellCasting, Verbose, TEXT("Cooldown Removed"));
	}
	else
	{

		cooldownMap[ElementToRemove].instances.RemoveAt(0, 1, true);

	}

	buffDebuffArray.RemoveAt(indexToRemove, 1, true);


}


///////////////////////////////////////////////////////
// Helper Functions

void UCooldownComponent::addToCooldownArray(float endTime, FName elementToAdd)
{
	FAbilitybuffDebuffArrayElement newCooldown;
	newCooldown.endTime = endTime;
	newCooldown.elementName = elementToAdd;
	buffDebuffArray.Add(newCooldown);
	if (buffDebuffArray.Num() > 1)
	{
		sortCooldownArray();
	}

}

void UCooldownComponent::addToCooldownMap(float startTime, float endTime, FName elementToAdd)
{

	FAbilityCooldownElement newCharge;
	newCharge.startTime = world->GetTimeSeconds();
	newCharge.endTime = endTime;

	// if the spell already exists in the cooldown array (multi-charge spell, we are putting another charge on cooldown)
	if (cooldownMap.Contains(elementToAdd))
	{

		cooldownMap[elementToAdd].instances.Add(newCharge);

		if (cooldownMap[elementToAdd].count() > 1)
		{
			cooldownMap[elementToAdd].sortInstances();
		}

	}
	// the spell does not exist in the cooldown array, this is the first instance we've seen of it
	else
	{

		FAbilityCooldownContainer newCooldownEntry;
		newCooldownEntry.instances.Add(newCharge);
		cooldownMap.Add(elementToAdd);
		cooldownMap[elementToAdd] = newCooldownEntry;
	}
}

int32 UCooldownComponent::getFirstIndexOfElementName(FName element)
{
	for (int i = 0; i < buffDebuffArray.Num(); i++)
	{
		if (buffDebuffArray[i].elementName == element)
		{
			return i;
		}

	}
	return -1;
}

FAbilityCooldownElement UCooldownComponent::getElementInstanceLeastTimeRemaining(FName element)
{
	return cooldownMap.Find(element)->instances[0];
}


FAbilityCooldownContainer UCooldownComponent::getElementData(FName element)
{
	FAbilityCooldownContainer * FoundElement = cooldownMap.Find(element);
	return *FoundElement;

}

// sorts the buffDebuffArray such that the element with the nearest endtime is at position 0 
void UCooldownComponent::sortCooldownArray()
{

	int j;
	FAbilitybuffDebuffArrayElement temp;

	for (int i = 0; i < buffDebuffArray.Num(); i++)
	{
		j = i;

		while (j > 0 && buffDebuffArray[j].endTime < buffDebuffArray[j - 1].endTime)
		{
			temp = buffDebuffArray[j];
			buffDebuffArray[j] = buffDebuffArray[j - 1];
			buffDebuffArray[j - 1] = temp;
			j--;
		}
	}

}

int32 UCooldownComponent::getNumberOfInstancesOfElement(FName element)
{
	if (cooldownMap.Find(element))
	{

		return cooldownMap.Find(element)->count();

	}
	else
	{
		return 0;
	}

}


// tests the first element of the array (one with smallest endtime after sorting), if the timer has expired, remove it
// is used by timer
void UCooldownComponent::pingBuffDebuffArray()
{

	//if there are no elements in the cooldown array, stop the timer and return
	if (buffDebuffArray.Num() == 0)
	{

		world->GetTimerManager().ClearTimer(cooldownTimer);
		return;
	}

	if (buffDebuffArray[0].endTime <= world->GetTimeSeconds())
	{

		removeFromCooldown(buffDebuffArray[0].elementName);
	}

}
