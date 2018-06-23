// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ability_Master.h"
#include "TargetTypes.h"
#include "AbilityTargeting.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataReady, const FTargetDataHandle&, Data);

UCLASS(Abstract)
class NETWORKINGTESTCPP_API AAbilityTargeting : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityTargeting();

	FTargetDataReady SendData;

	UFUNCTION(BlueprintImplementableEvent, Category = "Targeting")
	void StartTargeting(AAbility_Master* TargetingAbility);

	UFUNCTION(BlueprintImplementableEvent, Category = "Targeting")
	void ConfirmTargeting();

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void BroadcastData(FTargetDataHandle Handle);

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void EndTargeting();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
