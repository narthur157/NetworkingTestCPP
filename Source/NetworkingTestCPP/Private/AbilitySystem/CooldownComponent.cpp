// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/CooldownComponent.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UCooldownComponent::UCooldownComponent()
{
	
}

void UCooldownComponent::Init(UWorld * World)
{
	world = World;
	
}

void UCooldownComponent::placeTagOnCooldown(FName ElementToAdd, float Duration)
{
	float startTime = world->GetTimeSeconds();
	float endTime = startTime + Duration;

	addToCooldownArray(endTime, ElementToAdd);
	addToCooldownMap(startTime, endTime, ElementToAdd);
	newCooldown.Broadcast(ElementToAdd);
	
	if (world->GetTimerManager().IsTimerActive(cooldownTimer) != true)
	{

		FTimerDelegate cooldownTick;
		cooldownTick.BindUFunction(this, FName("pingCooldownArray"));
		world->GetTimerManager().SetTimer(cooldownTimer, cooldownTick, .01, true, 0);
	
	}

}

void UCooldownComponent::removeShortestChargeFromCooldown(FName ElementToRemove, int32 NumChargesToRemove)
{
	
	for (int i = 0; i < NumChargesToRemove; i++)
	{
		
		int32 indexToRemove = getFirstIndexOfElement(ElementToRemove);
		// if there exists an element in cooldown array
		if (indexToRemove != -1)
		{

			cooldownArray.RemoveAt(indexToRemove, 1, true);
			FAbilityCooldownContainer* ContainerToRemoveFrom = cooldownMap.Find(ElementToRemove);
			ContainerToRemoveFrom->instances.RemoveAt(0, 1, true);
			
		}
		// otherwise, there are no more charges to remove
		else
		{
			break;
		}
	}
}

void UCooldownComponent::removeLongestChargeFromCooldown(FName ElementToRemove, int32 NumChargesToRemove)
{

	for (int i = cooldownArray.Num()-1; i >= 0 ; i--)
	{

		int32 indexToRemove = getLastIndexOfElement(ElementToRemove);
		// if there exists an element in cooldown array
		if (indexToRemove != -1)
		{
			cooldownArray.RemoveAt(indexToRemove, 1, true);
			FAbilityCooldownContainer* ContainerToRemoveFrom = cooldownMap.Find(ElementToRemove);
			ContainerToRemoveFrom->instances.RemoveAt(ContainerToRemoveFrom->count(), 1, false);

		}
		// otherwise, there are no more charges to remove
		else
		{
			break;
		}


	}

}


///////////////////////////////////////////////////////
// Helper Functions

void UCooldownComponent::addToCooldownArray(float endTime, FName elementToAdd)
{
	FAbilityCooldownArrayElement newCooldownElement;
	newCooldownElement.endTime = endTime;
	newCooldownElement.elementName = elementToAdd;
	cooldownArray.Add(newCooldownElement);
	if (cooldownArray.Num() > 1)
	{
		sortCooldownArray();
	}

}

int32 UCooldownComponent::getLastIndexOfElement(FName element)
{

	for (int i = cooldownArray.Num(); i >= 0; i--)
	{
		if (cooldownArray[i].elementName == element)
		{
			return i;
		}

	}
	return -1;

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
		newCooldownEntry.ContainerName = elementToAdd;
		newCooldownEntry.instances.Add(newCharge);
		cooldownMap.Add(elementToAdd);
		cooldownMap[elementToAdd] = newCooldownEntry;
	}
}

int32 UCooldownComponent::getFirstIndexOfElement(FName element)
{
	for (int i = 0; i < cooldownArray.Num(); i++)
	{
		if (cooldownArray[i].elementName == element)
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


FAbilityCooldownElement UCooldownComponent::getElementInstanceMostTimeRemaining(FName element)
{

	return cooldownMap.Find(element)->instances[getNumberOfInstancesOfElement(element) - 1];

}



FAbilityCooldownContainer UCooldownComponent::getElementData(FName element)
{
	
	FAbilityCooldownContainer FoundCooldown;
	FoundCooldown = cooldownMap.FindRef(element);
	
	return FoundCooldown;

}

// sorts the cooldownArray such that the element with the nearest endtime is at position 0 
void UCooldownComponent::sortCooldownArray()
{

	int j;
	FAbilityCooldownArrayElement temp;

	for (int i = 0; i < cooldownArray.Num(); i++)
	{
		j = i;

		while (j > 0 && cooldownArray[j].endTime < cooldownArray[j - 1].endTime)
		{
			temp = cooldownArray[j];
			cooldownArray[j] = cooldownArray[j - 1];
			cooldownArray[j - 1] = temp;
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
// timer delegate function
void UCooldownComponent::pingCooldownArray()
{

	//if there are no elements in the cooldown array, stop the timer and return
	if (cooldownArray.Num() == 0)
	{

		world->GetTimerManager().ClearTimer(cooldownTimer);
		return;
	}

	if (cooldownArray[0].endTime <= world->GetTimeSeconds())
	{

		removeShortestChargeFromCooldown(cooldownArray[0].elementName, 1);
	}

	cooldownUpdate.Broadcast();
}
