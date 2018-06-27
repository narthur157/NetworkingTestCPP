// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability_Master.h"
#include "CooldownComponent.generated.h"


struct FAbilitybuffDebuffArrayElement;
struct FAbilityCooldownElement;
struct FAbilityCooldownContainer;
class UCooldownComponent;


USTRUCT(BlueprintType)
struct FAbilityCooldownArrayElement
{

	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName elementName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float endTime;

};


USTRUCT(BlueprintType)
struct FAbilityCooldownElement
{

	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float startTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float endTime;


};

USTRUCT(BlueprintType)
struct FAbilityCooldownContainer
{

	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName ContainerName = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FAbilityCooldownElement> instances;

	int32 count()
	{
		return instances.Num();

	}

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

	bool isValidCooldown()
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

/* ---------------Cooldown Delegates--------------- */
/* Fired when the cooldown manager has sucesfully added a new cooldown */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewCooldown, FName, CooldownName);
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
	
	UPROPERTY(BlueprintAssignable)
	FNewCooldown newCooldown;

	UPROPERTY(BlueprintAssignable)
	FCooldownTick cooldownUpdate;


	FAbilityCooldownContainer getElementData(FName element);
	//name 
	FAbilityCooldownElement getElementInstanceLeastTimeRemaining(FName element);
	FAbilityCooldownElement getElementInstanceMostTimeRemaining(FName element);
	int32 getNumberOfInstancesOfElement(FName element);
	void placeTagOnCooldown(FName ElementToAdd, float Duration);
	void removeShortestChargeFromCooldown(FName ElementToRemove, int32 NumChargesToRemove);
	void removeLongestChargeFromCooldown(FName ElementToRemove, int32 NumChargesToRemove);

	void Init(UWorld* World);
	
private:
	//world which the spellbook exists in
	UWorld * world;
	

	// need cooldowns with the least amount of time at index 0
	void sortCooldownArray();

	FTimerHandle cooldownTimer;

	void addToCooldownArray(float endTime, FName elementToAdd);
	void addToCooldownMap(float startTime, float endTime, FName elementToAdd);

	int32 getFirstIndexOfElement(FName element);
	int32 getLastIndexOfElement(FName element);

	UFUNCTION()
	void pingCooldownArray();

	//storage 
	TArray<FAbilityCooldownArrayElement> cooldownArray;
	TMap<FName, FAbilityCooldownContainer> cooldownMap;

};
