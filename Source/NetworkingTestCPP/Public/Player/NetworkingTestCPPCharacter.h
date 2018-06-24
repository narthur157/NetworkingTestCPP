// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player/S_PlayerStats.h"
#include "AbilityUser.h"
#include "NetworkingTestCPPCharacter.generated.h"


UCLASS(config=Game)
class ANetworkingTestCPPCharacter : public ACharacter, public IAbilityUser
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

public:
	ANetworkingTestCPPCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory", Replicated)
		class USpellBookComponent* SpellBook;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseLookUpRate;

	// set player action state
	UFUNCTION(BlueprintCallable, Category = "StateActionHelper")
		void setPlayerActionState(EPlayerActionState newState);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_setPlayerActionState(EPlayerActionState newState);
		virtual void Server_setPlayerActionState_Implementation(EPlayerActionState newState);
		virtual bool Server_setPlayerActionState_Validate(EPlayerActionState newState);

	// set player movement state
	UFUNCTION(BlueprintCallable, Category = "StateMovementHelper")
		void setPlayerMovementState(EPlayerMovementState newState);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_setPlayerMovementState(EPlayerMovementState newState);
		virtual void Server_setPlayerMovementState_Implementation(EPlayerMovementState newState);
		virtual bool Server_setPlayerMovementState_Validate(EPlayerMovementState newState);

	// determines that if the character is casting
	UFUNCTION(BlueprintPure, Category = "StateActionHelper")
		bool isCasting();

	// determines that if the character is targeting
	UFUNCTION(BlueprintPure, Category = "StateActionHelper")
		bool isTargeting();

	// determines that if the character is targeting
	UFUNCTION(BlueprintPure, Category = "StateActionHelper")
		bool isIdle();

	// determines if the character is disabled
	UFUNCTION(BlueprintPure, Category = "StateMovementHelper")
		bool isDisabled();

	// gets playercontroller ID (server only)
	UFUNCTION(BlueprintPure, Category = "StateMovementHelper")
		int32 getPlayerID();

	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
		void onAbilityCancelled(const AAbility_Master* ability);
		virtual void onAbilityCancelled_Implementation(const AAbility_Master* ability) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
		void onAbilityCompleted(const AAbility_Master* ability);
		virtual void onAbilityCompleted_Implementation(const AAbility_Master* ability) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Abiliy")
		void onAbilityTargeting(const AAbility_Master* ability);
		virtual void onAbilityTargeting_Implementation(const AAbility_Master* ability) override;

	//init casting
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
		void onCasting(FName CastName, float duration, bool bShouldCountUp);
		virtual void onCasting_Implementation(FName CastName, float duration, bool bShouldCountUp);


	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
		bool CanUseAbility(const AAbility_Master* AttemptedAbility);
		virtual bool CanUseAbility_Implementation(const AAbility_Master* AttemptedAbility) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
		FPlayerBaseStats CharacterStats;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

