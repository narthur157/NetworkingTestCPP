// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/E_ResourceTypes.h"
#include "NetworkingTestCPP.h"
#include "ExtendableTimer.h"
#include "Ability_Master.generated.h"

enum ESpellState;
struct FAbilityStage;
struct FAbilityBase;


UENUM(BlueprintType)
enum EAbilityState
{

	AbilityIdle				UMETA(DisplayName = "Idle"),
	AbilityTargeting		UMETA(DisplayName = "Targeting"),
	AbilityCasting			UMETA(DisplayName = "Casting"),
	AbilityRunning			UMETA(DisplayName = "Running")


};

UENUM(BlueprintType)
enum EAbilityInstancingType
{
	NeverInstance			UMETA(DisplayName = "Never Instance"),
	InstanceOnActivation	UMETA(DisplayName ="Instance On Activation"),
	KeepInstanced			UMETA(DisplayName = "Always Instanced")


};

UENUM(BlueprintType)
enum EAbilityTargetType
{
	Self					UMETA(DisplayName = "Self"),
	GroundAOE				UMETA(DisplayName = "GroundAOE"),
	Projectile				UMETA(DisplayName = "Projectile"),
	Lockon					UMETA(DisplayName = "Lockon")

};


USTRUCT(BlueprintType)
struct FAbilityInputRules
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
	bool canUpdateTargeting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
	bool canCancel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
	bool lockMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool lockRotationYaw;

};

//contains variables which may change as user increases a spell's level
USTRUCT(BlueprintType)
struct FAbilityStage
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 charges = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float castTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FName firingSocketOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FAbilityInputRules castingInputRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FAbilityInputRules runningInputRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TMap<FName, int32> AbilityBlockingTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UTexture2D* overrideAbilityIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UAnimMontage* overrideTargetingAnimationSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		UAnimMontage* overrideCastingAnimationSequence;

	


};


//contains information which stays the same regardless of spell level
USTRUCT(BlueprintType)
struct FAbilityBase
{

	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpellBase")
		int32 abilityID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellBase")
		FName abilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellBase")
		int32 rank = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		TEnumAsByte<EAbilityTargetType> TargetingType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellBase")
		TEnumAsByte<EResourceTypes> resourceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		TEnumAsByte<EAbilityInstancingType> InstancingBehavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FName firingSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		UTexture2D* abilityIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		UAnimMontage* castingAnimationSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		UAnimMontage* targetingAnimationSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellBase")
		TArray<FAbilityStage> abilityStages;



};


UCLASS(Blueprintable)
class NETWORKINGTESTCPP_API AAbility_Master : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbility_Master();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AbilityParameters")
	FAbilityBase abilityParameters;

	UPROPERTY(BlueprintReadWrite, Category = "AbilityParameters")
	USpellBookComponent* owningSpellBook;

	UPROPERTY(BlueprintReadWrite, Category = "AbilityParameters")
	AController* owningController;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	AActor* targetingActor;

	void setAbilityState(EAbilityState newState);
	
	UFUNCTION(BlueprintPure, Category = "AbilityHelper")
	USkeletalMeshComponent* getInstigatorMesh();

public:	

	virtual void Tick(float DeltaTime) override;

	///////////////////////////////////////////////
	// Ability Helpers
	UFUNCTION(BlueprintCallable, Category = "AbilityHelper")
	UTargetingComponent * getCurrentTargetingComponent();

	UFUNCTION(BlueprintCallable, Category = "AbilityHelper")
	void setAbilityID(int32 abilityID);

	UFUNCTION(BlueprintPure, Category = "AbilityHelper")
	int32 getAbilityID();

	UFUNCTION(BlueprintPure, Category = "AbilityHelper")
	USpellBookComponent* getSpellBook();

	UFUNCTION(BlueprintCallable, Category = "AbilityHelper")
	void setRank(int32 rank);
	
	UFUNCTION(BlueprintPure, Category = "AbilityHelper")
	int32 getRank();

	UFUNCTION(BlueprintPure, Category = "AbilityHelper")
	EResourceTypes getResourceType();

	UFUNCTION(BlueprintPure, Category = "AbilityHelper")
	EAbilityInstancingType getInstancingType();

	UFUNCTION(BlueprintPure, Category = "AbilityHelper")
	FName getAbilityName();

	UFUNCTION(BlueprintPure, Category = "AbilityHelper")
	UTexture2D* getAbilityIcon();

	UFUNCTION(BlueprintPure, Category = "AbilityHelper")
	FAbilityStage getCurrentStage();
	
	UFUNCTION(BlueprintPure, Category = "AbilityHelper")
	UAnimMontage* getTargetingAnimation();

	UFUNCTION(BlueprintPure, Category = "AbilityHelper")
	UAnimMontage* getCastingAnimation();

	UFUNCTION(BlueprintCallable, Category = "AbilityHelper")
	EAbilityState getAbilityState();

	UFUNCTION(BlueprintPure, Category = "AbilityHelper")
	FName getFiringSocket();

	UFUNCTION(BlueprintPure)
	UExtendableTimer* getTimelineComponent();

	UFUNCTION(BlueprintCallable, Category = "AbilityHelper")
	void ExtendCurrentCastingOperation(float SecondsToExtend);

	UFUNCTION(BlueprintCallable, Category = "AbilityHelper")
	void ReduceCurrentCastingOperation(float SecondsToReduce);

	UFUNCTION()
	void setTimelineComponent(UExtendableTimer * Timer)
	{

		CastingTimeline = Timer;
	}


	UFUNCTION(BlueprintCallable, Category = "AbilityHelper")
	void PlayAnimation(UAnimMontage* MontageToPlay, float PlayRate);

	UFUNCTION(BlueprintCallable, Category = "AbilityHelper")
	void StopAnimation(UAnimMontage* MontageToStop);

	////////////////////////////////////////////////////////////////
	// Ability Run
	UFUNCTION(BlueprintCallable, Category = "AbilityRun")
	void run();
	UFUNCTION(BlueprintImplementableEvent, Category = "AbilityRun")
	void onRun();

	UFUNCTION(BlueprintCallable, Category = "AbilityRun")
	void castComplete();
	UFUNCTION(BlueprintImplementableEvent, Category = "AbilityRun")
	void onCastComplete();

private:
	UTargetingComponent * currentTargetingComponent;

	TEnumAsByte<EAbilityState> abilityState = AbilityIdle;

	UExtendableTimer* CastingTimeline;
};
