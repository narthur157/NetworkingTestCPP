// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityTargeting.h"


// Sets default values
AAbilityTargeting::AAbilityTargeting()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AAbilityTargeting::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	
}

// Called every frame
void AAbilityTargeting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAbilityTargeting::EndTargeting()
{

	Destroy(this);

}

void AAbilityTargeting::BroadcastData(FTargetDataHandle Handle)
{
	
	SendData.Broadcast(Handle);

	EndTargeting();
}