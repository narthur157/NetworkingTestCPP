// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/NetworkingTestCPPCharacter.h"
#include "NetworkingTestCPP.h"
#include "AbilitySystem/Ability_Master.h"
#include "TargetTypes.h"
#include "CooldownComponent.h"
#include "SpellBookComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FValidTargetDataReady, const FTargetDataHandle&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FConfirmAbility);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCastingUpdate, float, TimeElapsed, float, TotalDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCastingBegin, AAbility_Master*, CastingAbility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCastCompleted, AAbility_Master*, CastingAbility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCastCancelled, AAbility_Master*, CastingAbility);


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetingBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetingCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetingCancelled);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayAnimation, UAnimMontage*, Anim, float, PlayRate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStopAnimation, UAnimMontage*, Anim);




USTRUCT(BlueprintType)
struct FKnownAbility
{

	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 abilityID;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 abilityRank;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AAbility_Master * abilityInstance;


	// move this into being a spellcomponent function tbh
	void updateAbilityRank(int32 newRank)
	{

		this->abilityRank = newRank;
		if (this->abilityInstance != nullptr)
		{

			//abilityInstance->setRank(newRank);

		}

	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NETWORKINGTESTCPP_API USpellBookComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	
	FConfirmAbility ConfirmAbility;
	FValidTargetDataReady TargetDispatcher;

	UPROPERTY(BlueprintAssignable)
	FCastingBegin CastBegun;

	UPROPERTY(BlueprintAssignable)
	FCastingUpdate CastingBarUpdate;

	UPROPERTY(BlueprintAssignable)
	FCastCompleted CastCompleted;

	UPROPERTY(BlueprintAssignable)
	FCastCancelled CastCancelled;

	UPROPERTY(BlueprintAssignable)
	FPlayAnimation PlayAnimation;

	UPROPERTY(BlueprintAssignable)
	FStopAnimation StopAnimation;

	UPROPERTY(BlueprintAssignable)
	FTargetingBegin TargetingBegin;

	UPROPERTY(BlueprintAssignable)
	FTargetingCompleted TargetingConfirmed;

	UPROPERTY(BlueprintAssignable)
	FTargetingCancelled TargetingCancelled;

	

	// Sets default values for this component's properties
	USpellBookComponent();
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	// AllPossibleSpells. needs to be public now because we arent importing from a database or some shit and manually setting it up in editor
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpellCastVars")
	TMap<int32, TSubclassOf<AAbility_Master>> allPossibleAbilities;

	/////////////////////////////////////////////////////////////////////
	// TryCast Functions
	// return a KnownSpell index if the abilityID is valid and the player can cast
	UFUNCTION(BlueprintCallable, Category = "TrySpellCast")
	int32 canUseAbility(int32 abilityID);

	UFUNCTION(BlueprintCallable, Category = "TrySpellCast")
	bool isOnCooldown(int32 abilityID);

	UFUNCTION(BlueprintCallable, Category = "TrySpellCast")
	bool isKnown(int32 abilityID);

	UFUNCTION(BlueprintCallable, Category = "TrySpellCast")
	bool hasResourcesToCast(AAbility_Master* Spell, ANetworkingTestCPPCharacter* Caster);

	/////////////////////////////////////////////////////////////////////
	// KnownAbilities operations
	// AddToKnownAbilitiesArray
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "KnownAbilities")
	void addToKnownAbilities(int32 abilityID, int32 rank);
	virtual void addToKnownAbilities_Implementation(int32 abilityID, int32 rank);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_addToKnownAbilities(int32 abilityID, int32 rank);

	// RemoveFromKnownAbilitiesArray
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "KnownAbilities")
	void removeFromKnownAbilities(int32 abilityID);
	virtual void removeFromKnownAbilities_Implementation(int32 abilityID);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_removeFromKnownAbilities(int32 abilityID);
	
	/////////////////////////////////////////////////////////////////////
	// Cooldown Operation
	UFUNCTION(BlueprintPure, Category = "CooldownManagement")
	UCooldownComponent * getCooldownManager();

	UFUNCTION(BlueprintCallable, Category = "CooldownManagement")
	void placeAbilityOnCooldown(AAbility_Master* abilityToPlace);

	UFUNCTION(BlueprintCallable, Category = "CooldownManagement")
	void placeAbilityOnCooldownWithModifier(AAbility_Master* abilityToPlace, float CooldownModifier);

	/////////////////////////////////////////////////////////////////////
	// Ability Operations
	// UseAbility
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AbilityUse")
	void UseAbility(int32 abilityID);
	virtual void UseAbility_Implementation(int32 abilityID);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UseAbility(int32 abilityID);

	UFUNCTION(BlueprintCallable, Category = "AbilityUse")
	void commitAbility();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AbilityUse")
	void cancelAbility();
	virtual void cancelAbility_Implementation();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_cancelAbility();

	UFUNCTION(BlueprintCallable, Category = "AbilityUse")
	void endAbility(AAbility_Master * abilityToEnd);


	/////////////////////////////////////////////////////////////////////
	//HelperFunction
	UFUNCTION(BlueprintPure, Category = "SpellCast")
	AAbility_Master * getCurrentlyCastingAbility();
	UFUNCTION(BlueprintCallable, Category = "SpellDataVarsHelper")
	void applyAbilityInputRules(FAbilityInputRules newRules);
	UFUNCTION(BlueprintPure, Category = "KnownAbilities")
	int32 getAbilityRank(int32 abilityID);

	// returns "None" if ability ID is not valid
	UFUNCTION(BlueprintPure, Category = "AbilityHelper")
	FName getAbilityNameFromID(int32 abilityID);

	// Fires valid data 
	UFUNCTION()
	void ValidateAndBroadcastValidData(const FTargetDataHandle &Data);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ValidateAndBroadcastValidData(const FTargetDataHandle &Data);


protected:

	// Called when the game starts
	virtual void BeginPlay() override;


	//abilityID, Class, Rank 
	UPROPERTY(BlueprintReadOnly,  Category = "SpellCastVars", Replicated)
	TArray<FKnownAbility> knownAbilities;

	UFUNCTION()
	int32 findInKnownAbilities(int32 abilityID);


private:

	AAbility_Master* currentlyCastingAbility;
	UCooldownComponent * cooldownManager;
	

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
};
