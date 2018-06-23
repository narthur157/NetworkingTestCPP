// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability_Master.h"
#include "Engine/World.h"
#include "Player/NetworkingTestCPPCharacter.h"
#include "AbilitySystem/SpellBookComponent.h"
#include "AbilitySystem/TargetingComponent.h"
#include "EngineGlobals.h"


// Sets default values
AAbility_Master::AAbility_Master()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
}

// Called when the game starts or when spawned
void AAbility_Master::BeginPlay()
{
	Super::BeginPlay();
	owningSpellBook = getSpellBook();
	owningController = GetInstigatorController();

}

// Called every frame
void AAbility_Master::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAbility_Master::setRank(int32 rank)
{

	abilityParameters.rank = rank;

}

void AAbility_Master::setAbilityID(int32 abilityID)
{
	
	abilityParameters.abilityID = abilityID;

}



UAnimMontage* AAbility_Master::getTargetingAnimation()
{

	if (getCurrentStage().overrideTargetingAnimationSequence != nullptr)
	{
	
		return getCurrentStage().overrideTargetingAnimationSequence;
	}
	else
	{
		return abilityParameters.targetingAnimationSequence;

	}
}

UAnimMontage* AAbility_Master::getCastingAnimation()
{

	if (getCurrentStage().overrideCastingAnimationSequence != nullptr)
	{
	
		return getCurrentStage().overrideCastingAnimationSequence;
	}
	else
	{
	
		return abilityParameters.castingAnimationSequence;

	}
}



USpellBookComponent* AAbility_Master::getSpellBook()
{
	owningSpellBook = GetInstigator()->FindComponentByClass<USpellBookComponent>();
	if (owningSpellBook)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Found SpellBook"));
		return owningSpellBook;
	}
	
	return nullptr;
}

USkeletalMeshComponent* AAbility_Master::getInstigatorMesh()
{

	return owningController->GetCharacter()->GetMesh();

}

int32 AAbility_Master::getAbilityID()
{

	return abilityParameters.abilityID;

}

FName AAbility_Master::getAbilityName()
{

	return abilityParameters.abilityName;

}

EAbilityInstancingType AAbility_Master::getInstancingType()
{

	return abilityParameters.InstancingBehavior;
}

EAbilityState AAbility_Master::getAbilityState()
{

	return abilityState;

}

void AAbility_Master::setAbilityState(EAbilityState newState)
{

	abilityState = newState;

}

EResourceTypes AAbility_Master::getResourceType()
{

	return abilityParameters.resourceType;
}

FAbilityStage AAbility_Master::getCurrentStage()
{

	return abilityParameters.abilityStages[abilityParameters.rank];
}

int32 AAbility_Master::getRank()
{

	return abilityParameters.rank;

}

UExtendableTimer* AAbility_Master::getTimelineComponent()
{
	return CastingTimeline;
}

void AAbility_Master::PlayAnimation(UAnimMontage* MontageToPlay, float PlayRate)
{
	getSpellBook()->PlayAnimation.Broadcast(MontageToPlay, PlayRate);

}

void AAbility_Master::StopAnimation(UAnimMontage* MontageToPlay)
{
	getSpellBook()->StopAnimation.Broadcast(MontageToPlay);

}

UTargetingComponent * AAbility_Master::getCurrentTargetingComponent()
{

	return currentTargetingComponent;

}

FName AAbility_Master::getFiringSocket()
{

	if (getCurrentStage().firingSocketOverride != NAME_None)
	{
		return getCurrentStage().firingSocketOverride;

	}
	else
	{
		return abilityParameters.firingSocket;

	}

}

void AAbility_Master::ExtendCurrentCastingOperation(float SecondsToExtend)
{
	if (CastingTimeline)
	{
		CastingTimeline->ExtendTimerDuration(SecondsToExtend);
	}

}

void AAbility_Master::ReduceCurrentCastingOperation(float SecondsToReduce)
{
	if (CastingTimeline)
	{
		CastingTimeline->ReduceTimerDuration(SecondsToReduce);
	}

}


void AAbility_Master::run()
{

	onRun();
}

void AAbility_Master::castComplete()
{

	setAbilityState(EAbilityState::AbilityIdle);
	onCastComplete();

}
