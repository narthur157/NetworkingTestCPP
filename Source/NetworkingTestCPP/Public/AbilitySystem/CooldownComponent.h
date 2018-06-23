// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability_Master.h"
#include "CooldownComponent.generated.h"


USTRUCT(BlueprintType)
struct FAbilitybuffDebuffArrayElement
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


		
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NETWORKINGTESTCPP_API UCooldownComponent : public UObject
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCooldownComponent();

protected:

public:	
	

	UFUNCTION(BlueprintPure, Category = "CooldownManagement")
	FAbilityCooldownContainer getElementData(FName element);

	UFUNCTION(BlueprintPure, Category = "CooldownManagement")
	FAbilityCooldownElement getElementInstanceLeastTimeRemaining(FName element);

	UFUNCTION(BlueprintPure, Category = "CooldownManagement")
	int32 getNumberOfInstancesOfElement(FName element);

	UFUNCTION(BlueprintCallable, Category = "CooldownManagement")
	void placeTagOnCooldown(FName ElementToAdd, float Duration);

	UFUNCTION(BlueprintCallable, Category = "CooldownManagement")
	void removeFromCooldown(FName ElementToRemove);

	void setParams(UWorld * newWorld);
	
private:
	//world which the spellbook exists in
	UWorld * world;
	// need cooldowns with the least amount of time at index 0
	void sortCooldownArray();

	FTimerHandle cooldownTimer;

	void addToCooldownArray(float endTime, FName elementToAdd);
	void addToCooldownMap(float startTime, float endTime, FName elementToAdd);

	int32 getFirstIndexOfElementName(FName element);

	UFUNCTION()
	void pingBuffDebuffArray();

	//storage 
	TArray<FAbilitybuffDebuffArrayElement> buffDebuffArray;
	TMap<FName, FAbilityCooldownContainer> cooldownMap;

};
