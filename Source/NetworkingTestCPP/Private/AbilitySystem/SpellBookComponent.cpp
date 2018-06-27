#include "AbilitySystem/SpellBookComponent.h"
#include "UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Data/E_ResourceTypes.h"
#include "Player/S_PlayerStats.h"
#include "EngineGlobals.h"



// Sets default values for this component's properties
USpellBookComponent::USpellBookComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	SpellBookOwner = Cast<IAbilityUser>(GetOwner());
	// Create a manager for ability cooldowns
	cooldownManager = NewObject<UCooldownComponent>();
	cooldownManager->Init(GetWorld());


	//flag component for replication
	bReplicates = true;


}

// Tell UE to replicate variables
void USpellBookComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USpellBookComponent, knownAbilities);
}

// Called when the game starts
void USpellBookComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...

}

// Called every frame
void USpellBookComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/////////////////////////////////////////////////////////////////////
// Trycast
// returns - 1 if player cannot cast the spell at abilityID, returns the knownAbility index if player can cast
int32 USpellBookComponent::canUseAbility(int32 abilityID)
{
	
	//make sure spell is known by looking up ID in knownAbilityIndex, -1 if not known
	int32 knownAbilityIndex = -1;
	knownAbilityIndex = findInKnownAbilities(abilityID);
	if (knownAbilityIndex == -1)
	{
		UE_LOG(AbilitySystemInitialization, Verbose, TEXT("Ability %d failed CanCastSpell check, is not known"), abilityID);
		return -1;
	}

	//test if spell is on cooldown
	if (isOnCooldown(abilityID))
	{
		UE_LOG(AbilitySystemInitialization, Verbose, TEXT("Ability %d failed CanCastSpell check, is on cooldown"), abilityID);
		return -1;

	}

	TSubclassOf<AAbility_Master>* referencedSpell = allPossibleAbilities.Find(abilityID);
	if (!SpellBookOwner || !SpellBookOwner->Execute_CanUseAbility(GetOwner(), referencedSpell->GetDefaultObject()) )
	{

		if (!SpellBookOwner)
		{
			// should never get called
			UE_LOG(AbilitySystemInitialization, Fatal, TEXT("Ability %d failed CanCastSpell check, spellbook is invalid"), abilityID);
		}
		else
		{

			UE_LOG(AbilitySystemInitialization, Verbose, TEXT("Ability %d failed CanCastSpell check, owner defined CanUseAbility returned false"), abilityID);

		}
		return -1;
	}
	//character can cast
	else
	{
		return knownAbilityIndex;
	}

}

// tests to see if the spell is on cooldown, eventually this should test vs the number of charges that a spell has
bool USpellBookComponent::isOnCooldown(int32 abilityID)
{
	AAbility_Master* Ability = allPossibleAbilities.Find(abilityID)->GetDefaultObject();
	Ability->setRank(getAbilityRank(abilityID));
	if (!Ability)
	{
		return true;
	}

	// if the ability has a tag on cooldown, or 
	FAbilityCooldownContainer blockingElement = getBlockingElement(abilityID);
	if (blockingElement.isValidCooldown() && blockingElement.ContainerName != Ability->getAbilityName())
	{
		return true;
	}

	// if the ability has some charges on cooldown, but not all charges on cooldown
	else if (blockingElement.isValidCooldown() && blockingElement.ContainerName == Ability->getAbilityName() && blockingElement.count() == Ability->getCurrentStage().charges)
	{
		return true;
	}
	
	else
	{
		return false;
	}
}

//	test to see if ability is known
bool USpellBookComponent::isKnown(int32 abilityID)
{

	if (findInKnownAbilities(abilityID) != -1)
	{
		return true;
	}
	else
	{
		return false;
	}

}


UCooldownComponent* USpellBookComponent::getCooldownManager()
{
	
	return cooldownManager;
}


///////////////////////////////////////////////////////////////////////
// KnownAbilities Operations
// add to known spells
void USpellBookComponent::addToKnownAbilities_Implementation(int32 abilityID, int32 rank)
{

	if (GetOwnerRole() != ROLE_Authority)
	{

		Server_addToKnownAbilities(abilityID, rank);

	}

}

// server add to spells
void USpellBookComponent::Server_addToKnownAbilities_Implementation(int32 abilityID, int32 rank)
{

	// test to make sure the ability ID provided is valid
	TSubclassOf<AAbility_Master>* SpellToAdd = nullptr;
	SpellToAdd = allPossibleAbilities.Find(abilityID);

	if (SpellToAdd != nullptr)
	{
		if (findInKnownAbilities(abilityID) == -1)
		{

			FKnownAbility newAbility;
			newAbility.abilityID = abilityID;
			newAbility.abilityRank = rank;
			newAbility.abilityInstance = nullptr;
			knownAbilities.Add(newAbility);
			//UE_LOG(SpellCasting, Verbose, TEXT("Spell Added"));

		}
		else
		{

			//UE_LOG(SpellCasting, Verbose, TEXT("Spell already known"));

		}
	}
	else
	{

		//UE_LOG(SpellCasting, Verbose, TEXT("This spell doesn't exist"));
	}

}
bool USpellBookComponent::Server_addToKnownAbilities_Validate(int32 abilityID, int32 rank)
{
	return true;

}

// remove from known spells, edit this later
void USpellBookComponent::removeFromKnownAbilities_Implementation(int32 abilityID)
{


	if (GetOwnerRole() != ROLE_Authority)
	{

		Server_removeFromKnownAbilities(abilityID);

	}

	// test if spell exists
	int32 knownAbilityIndex = findInKnownAbilities(abilityID);

	// remove if exists
	if (knownAbilityIndex != -1) 
	{

		knownAbilities.RemoveAt(knownAbilityIndex);

	}

}


// remove from known spells implementation
void USpellBookComponent::Server_removeFromKnownAbilities_Implementation(int32 abilityID)
{

	removeFromKnownAbilities(abilityID);

}
bool USpellBookComponent::Server_removeFromKnownAbilities_Validate(int32 abilityID)
{
	return true;

}


// helper function to determine if player knows spell or not, return -1 on not found
int32 USpellBookComponent::findInKnownAbilities(int32 abilityID)
{

	int32 i = 0;
	for (i; i < knownAbilities.Num(); i++)
	{
		if (knownAbilities[i].abilityID == abilityID)
		{
			return i;
		}

	}

	return -1;

}

void USpellBookComponent::ValidateAndBroadcastValidData(const FTargetDataHandle &Data)
{

	if (GetOwnerRole() != ROLE_Authority)
	{

		Server_ValidateAndBroadcastValidData(Data);
		
	}

	TargetDispatcher.Broadcast(Data);


	//Validate data somehow


}

void USpellBookComponent::Server_ValidateAndBroadcastValidData_Implementation(const FTargetDataHandle &Data)
{
	ValidateAndBroadcastValidData(Data);

}

bool USpellBookComponent::Server_ValidateAndBroadcastValidData_Validate(const FTargetDataHandle &Data)
{

	return true;

}

// useful to test if an ability is on cooldown or not, returns the longest blocking element if the ability is on cooldown
// if none of an abilties blocking tags are on cooldown, then return number of instances of the abilities name (potential to be null as well)
FAbilityCooldownContainer USpellBookComponent::getBlockingElement(int32 AbilityID)
{
	FAbilityCooldownContainer blockingElement;
	TSubclassOf<AAbility_Master>* AbilityClass = allPossibleAbilities.Find(AbilityID);
	if (!AbilityClass)
	{
		//ability doesnt exist in allpossibleabilities
		return blockingElement;
	}

	AAbility_Master* Ability = AbilityClass->GetDefaultObject();
	Ability->setRank(getAbilityRank(AbilityID));
	Ability->getCurrentStage().AbilityBlockingTags;

	TMap<FName, int32> Tags = Ability->getCurrentStage().AbilityBlockingTags;
	TArray<FName> Keys;
	Tags.GetKeys(Keys);
	

	for(int i = 0; i < Keys.Num(); i++)
	{		
		FAbilityCooldownContainer testContainer;
		testContainer = cooldownManager->getElementData(Keys[i]);
		
		if (testContainer.isValidCooldown() && testContainer.count() == *Tags.Find(Keys[i]))
		{
			// if the element hasnt been set
			if (!blockingElement.isValidCooldown())
			{
				blockingElement = testContainer;
			}
			// if the new element has a later endtime than the current blockingElement endtime
			else if(blockingElement.instances[0].endTime < testContainer.instances[0].endTime)
			{
				blockingElement = testContainer;
			}
		}

	}

	if (!blockingElement.isValidCooldown())
	{

		return cooldownManager->getElementData(Ability->getAbilityName());
	}
	else
	{
		return blockingElement;
	}
}


//////////////////////////////////////////////////////////////////////
// Cooldown Operations
// Adds to spells on cooldown dict
void USpellBookComponent::placeAbilityOnCooldown(AAbility_Master* abilityToPlace)
{

	cooldownManager->placeTagOnCooldown(abilityToPlace->getAbilityName(), abilityToPlace->getCurrentStage().cooldown);

	// TODO:: make a delegate that fires when a cooldown is incurred, bind UI function to this new delegate
	

}

void USpellBookComponent::placeAbilityOnCooldownWithModifier(AAbility_Master* abilityToPlace, float CooldownModifier)
{
	cooldownManager->placeTagOnCooldown(abilityToPlace->getAbilityName(), abilityToPlace->getCurrentStage().cooldown * CooldownModifier);

	// TODO:: make a delegate that fires when a cooldown is incurred, bind UI function to this new delegate

}


///////////////////////////////////////////////////////////////
// Ability Operations
// Cast Spell based on passed ID
void USpellBookComponent::UseAbility_Implementation(int32 abilityID)
{

	// Cast on server if we are remote
	if (GetOwnerRole() != ROLE_Authority)
	{

		Server_UseAbility(abilityID);

	}

	//if the player can cast the spell (has enough resources, knows spell, isn't currently busy)
	int validKnownAbilityIndex = canUseAbility(abilityID);
	if (validKnownAbilityIndex != -1)
	{


		TSubclassOf <AAbility_Master> * spellToCast = allPossibleAbilities.Find(abilityID);
		AActor* CastingPlayer = GetOwner();
		FActorSpawnParameters SpellSpawnInfo;
		UWorld* const World = GetWorld();
		SpellSpawnInfo.Owner = CastingPlayer;
		SpellSpawnInfo.Instigator = Cast<APawn>(CastingPlayer);

		AAbility_Master* Spell = GetWorld()->SpawnActor<AAbility_Master>(*spellToCast, CastingPlayer->GetActorLocation(), CastingPlayer->GetActorRotation(), SpellSpawnInfo);

		Spell->setRank(knownAbilities[validKnownAbilityIndex].abilityRank);
		Spell->setAbilityID(abilityID);
		currentlyCastingAbility = Spell;
		//UE_LOG(SpellCasting, Verbose, TEXT("Initing spell"));

		Spell->run();


		// spawn spell object and run

	}
	//if spell doesnt exist in known spells
	else
	{

	}

}

// Cast Spell Replication implementation
void USpellBookComponent::Server_UseAbility_Implementation(int32 abilityID)
{
	UseAbility(abilityID);

}
bool USpellBookComponent::Server_UseAbility_Validate(int32 abilityID)
{
	return true;
}

void USpellBookComponent::commitAbility()
{
	ConfirmAbility.Broadcast();
	
}


void USpellBookComponent::cancelAbility_Implementation()
{

	if (GetOwner()->Role != ROLE_Authority)
	{
		Server_cancelAbility();

	}
	
	//TODO: Implement this to broadcast a "Cancel" event
	

}


void USpellBookComponent::Server_cancelAbility_Implementation()
{

	cancelAbility();

}

bool USpellBookComponent::Server_cancelAbility_Validate()
{
	return true;

}

void USpellBookComponent::endAbility(AAbility_Master * completedAbility)
{

	completedAbility->Destroy();

}


/////////////////////////////////////////////////////////////////////
// Helper Fuctions
int32 USpellBookComponent::getAbilityRank(int32 abilityID)
{
	int32 abilityIndex = findInKnownAbilities(abilityID);
	if (abilityIndex != -1)
	{
		return knownAbilities[abilityIndex].abilityRank;

	}
	else
	{
		return -1;
	}

}

AAbility_Master * USpellBookComponent::getCurrentlyCastingAbility()
{
	if (IsValid(currentlyCastingAbility))
	{
		return currentlyCastingAbility;
	}
	else
	{
		return nullptr;
	}

}

TArray<FName> USpellBookComponent::GetAbilityBlockingTags(int32 AbilityID)
{
	AAbility_Master* Ability = allPossibleAbilities.Find(AbilityID)->GetDefaultObject();
	TArray<FName> Keys;
	
	if (!Ability)
	{
		return Keys;
	}

	Ability->setRank(getAbilityRank(AbilityID));
	Ability->getCurrentStage().AbilityBlockingTags.GetKeys(Keys);

	return Keys;
}


// eventually spawn debuff on character that prevents movement if the spell requires the character to be rooted for the durationn of running/casting
// using ACharacter is here probably bad honestly, need to rework that eventually, also not a huge fan of casting to Anetworkingntestcppcharacter, maybe implement some event 
void USpellBookComponent::applyAbilityInputRules(FAbilityInputRules newRules)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Trying"));
	// only controllable units can cast (AI or Player)
	APawn* castingPawn = Cast<APawn>(GetOwner());
	if (castingPawn)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Found Pawn"));
		castingPawn->bUseControllerRotationYaw = !newRules.lockRotationYaw;
		if (newRules.lockMovement == true)
		{
			// this will need to be removed when we swap from ACharacter movement to APawn movement to allow for non-humanoids
			ACharacter* castingCharacter = castingPawn->GetController()->GetCharacter();
			if (castingCharacter)
			{

				castingCharacter->GetCharacterMovement()->DisableMovement();
				castingCharacter->GetCharacterMovement()->StopMovementImmediately();

			}

		}
		else
		{

			castingPawn->GetController()->GetCharacter()->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		}

	}
	
}

AAbility_Master* USpellBookComponent::getAbilityDefaultObject(int32 AbilityID)
{
	AAbility_Master* Ability = allPossibleAbilities.Find(AbilityID)->GetDefaultObject();
	if (!Ability)
	{
		return nullptr;
	}
	Ability->setRank(getAbilityRank(AbilityID));
	return Ability;
}

FName USpellBookComponent::getAbilityNameFromID(int32 abilityID)
{
	TSubclassOf<AAbility_Master> * abilityClassReference = allPossibleAbilities.Find(abilityID);
	if (abilityClassReference)
	{

		return abilityClassReference->GetDefaultObject()->getAbilityName();

	}
	else
	{
		return "None";
	}
}