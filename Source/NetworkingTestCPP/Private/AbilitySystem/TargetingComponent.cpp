// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/TargetingComponent.h"
#include "AbilitySystem/Ability_Master.h"

// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(false);
	
	
	// ...
}


// Called when the game starts
void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	owningAbility = Cast<AAbility_Master>(GetOwner());
	// ...
	
}


// Called every frame
void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UTargetingComponent::setTargetingInfo(FTargetingInfo newInfo)
{

	TargetingInfo = newInfo;

}
