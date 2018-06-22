// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"

class AAbility_Master;

USTRUCT(BlueprintType)
struct FTargetingInfo
{

	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* TargetedActor;


};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class NETWORKINGTESTCPP_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	FTargetingInfo TargetingInfo;
	UPROPERTY(BlueprintReadWrite)
	AActor * targetingVisuals;
	UPROPERTY(BlueprintReadWrite)
	AAbility_Master * owningAbility;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Targeting")
	void startTargeting(AActor * targetVisuals);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Targeting")
	void confirmAndEndTargeting();

	UFUNCTION(BlueprintPure, Category = "TargetingHelper")
	FTargetingInfo getTargetingInfo()
	{
		return TargetingInfo;

	}
		
	
	UFUNCTION(BlueprintCallable, Category = "TargetingHelper")
	void setTargetingInfo(FTargetingInfo newInfo);
	

};
