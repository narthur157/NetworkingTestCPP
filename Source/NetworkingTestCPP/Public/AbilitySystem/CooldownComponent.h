// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability_Master.h"
#include "CooldownComponent.generated.h"

struct FAbilityCooldownElement;
struct FAbilityCooldownContainer;
class UCooldownComponent;


/*-----------------------------------  CooldownManager  -----------------------------------*/
/* -Manages the disabling of elements                                                      */		
/* -Whenever the number of instances of an element in cooldownArray                        */
/*  is equal to the blocking amount specified by the ability, that ability is blocked      */
/* -Utilizes a ticking delegate to compare the current time vs the time of the element at  */
/*  index 0 of the sorted cooldownArray, if they are equal, that instance is removed       */
/* -Use cooldownMap for quick lookups to determine the state of any particular element     */
/* -Bind to cooldownUpdate to recieve broadcast on tick, bind to newCooldown to recieve    */
/*  broadcast on new cooldown                                                              */
/*-----------------------------------------------------------------------------------------*/

// Main building block of the cooldown system                                                                                               
// This could have been split up into two different structs (one without elementName for the map and one without startTime for the array    
// However I decided to increase readability/reduce confusion at the cost of 4bytes per element instance or whatever   
// @elementName 
// @startTime time that the element was added
// @endTime time that the element will expire
USTRUCT(BlueprintType)
struct FAbilityCooldownElement
{

	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName elementName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float startTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float endTime;


};

// Main cooldownMap element, is essentially a snapshot of the current state of the cooldown array
// Allows us to store multiple charges and quickly determine the state of an element
// @ContainerName Name of the element
// @instances Details about each of the instances of the element
USTRUCT(BlueprintType)
struct FAbilityCooldownContainer
{

	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName ContainerName = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FAbilityCooldownElement> instances;

	int32 count() const
	{
		return instances.Num();

	}

	/* insertionsort, similar to cooldownArray, this must be sorted whenever an element is added/changed */
	void sortInstances()
	{
		int j;
		FAbilityCooldownElement temp;

		for (int i = 0; i < count(); i++) {
			j = i;

			while (j > 0 && instances[j].endTime < instances[j - 1].endTime) {
				temp = instances[j];
				instances[j] = instances[j - 1];
				instances[j - 1] = temp;
				j--;
			}
		}
	}

	/* Since we cannot expose pointers to USTRUCTS to blueprint, we use this to determine if the cooldown is valid or is just a default (null) return */
	bool isValidCooldown() const
	{
		if (ContainerName == "")
		{
			return false;
		}
		else
		{
			return true;
		}
	}
		
};

// ---------------Cooldown Delegates--------------- 
// Fired when the cooldown manager has sucesfully added a new cooldown 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewCooldown, FName, CooldownName);
// Fired for each tick
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCooldownTick);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NETWORKINGTESTCPP_API UCooldownComponent : public UObject
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCooldownComponent();

protected:

public:	
	

	// Cooldown Delegates 
	// To be called whenever an element is added/changed so that updates can be made 
	UPROPERTY(BlueprintAssignable)
	FNewCooldown newCooldown;

	// To be called whenever we want the cooldownarray to be pinged 
	UPROPERTY(BlueprintAssignable)
	FCooldownTick cooldownUpdate;

	// Initializes the element, we need a world reference to use timers
	void Init(UWorld* World);

	// Outward facing interface to add an element to the disabled list 
	void placeTagOnCooldown(FName ElementToAdd, float Duration);
	

	// Helper Functions 
	// @Return the cooldown element or the default value of FAbilityCooldownContainer if the element doesnt exist 
	FAbilityCooldownContainer getElementData(FName element) const;

	// @Return the cooldown element with the least time remaining, or default FAbilityCooldownContainer if the element doesnt exist 
	FAbilityCooldownElement getElementInstanceLeastTimeRemaining(FName element) const;

	// @Return the cooldown element with the least time remaining, or default FAbilityCooldownContainer if the element doesnt exist 
	FAbilityCooldownElement getElementInstanceMostTimeRemaining(FName element) const ;

	// @Return the count of the elements 
	int32 getNumberOfInstancesOfElement(FName element) const;

	// Removes charges from cooldown, starting from the shortest element, defaults to 1 
	void removeShortestChargeFromCooldown(FName ElementToRemove, int32 NumChargesToRemove = 1);

	// Removes charges from cooldown, starting from the longest element, defaults to 1
	void removeLongestChargeFromCooldown(FName ElementToRemove, int32 NumChargesToRemove = 1);

	
private:
	//world which the spellbook exists in
	UWorld * world;
	
	// Sorts the cooldown array such that the element with the least duration (current time - endtime) remaining is in the first index
	void sortCooldownArray();

	// Timer which checks the first element of cooldownArray to determine if the element in position 0 has expired 
	FTimerHandle cooldownTimer;

	// Disables the tag, sorts cooldownarray/cooldownmap after adding
	// In the future, we might wana check the first element against the endtime being added and immediately place it in position 0
	void disableTag(float startTime, float endTime, FName elementToAdd);

	// Finds the first/last index of an element                                                                                        
	// Cooresponds to the first element in the CooldownMap instance array 
	// e.g. getFirstIndexOfElement("Fireball") is the same as cooldownMap.Find("Fireball").instances[0]
	// @Return -1 if element doesnt exist in the cooldownarray
	int32 getFirstIndexOfElement(FName element) const;

	// Finds the last index of an element
	// Cooresponds to the last element in the cooldownmap instance array
	// e.g. getFirstIndexOfElement("Fireball") is the same as cooldownMap.Find("Fireball").instances[0]
	// @Return -1 if element doesnt exist in the cooldownarray
	int32 getLastIndexOfElement(FName element) const;


	// Delegate bound function that checks to see if the first element of cooldownArray has expired (currenttime > cooldownarray[0].endtime 
	// Must be a UFUNCTION in order for the delegate to call it                                                                             
	UFUNCTION()
	void pingCooldownArray();

	// Main storage array. This array must remain sorted and any changes made must call sortCooldownArray(), as we are only checking the endtime of                 
	// the element at index 0 to see if it expired. This saves a tiny bit of time as we don't have to iterate through the entire array in order to kick elements    
	//	which have expired                                                                                                                                          
	TArray<FAbilityCooldownElement> cooldownArray;

	// Similar to cooldownArray, instances existing in cooldown map must be sorted whenever a change was made to an element 
	TMap<FName, FAbilityCooldownContainer> cooldownMap;

};
