// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NetworkingTestCPPCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Spell/SpellBookComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerState.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ANetworkingTestCPPCharacter

ANetworkingTestCPPCharacter::ANetworkingTestCPPCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	bReplicates = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//This Actor can cast spells
	SpellBook = CreateDefaultSubobject<USpellBookComponent>(TEXT("SpellComponent"));
	SpellBook->SetIsReplicated(true);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

// Replication
void ANetworkingTestCPPCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetworkingTestCPPCharacter, SpellBook);


}

//////////////////////////////////////////////////////////////////////////
// Input

void ANetworkingTestCPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ANetworkingTestCPPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANetworkingTestCPPCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ANetworkingTestCPPCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ANetworkingTestCPPCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ANetworkingTestCPPCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ANetworkingTestCPPCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ANetworkingTestCPPCharacter::OnResetVR);
}


void ANetworkingTestCPPCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ANetworkingTestCPPCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ANetworkingTestCPPCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ANetworkingTestCPPCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ANetworkingTestCPPCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ANetworkingTestCPPCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ANetworkingTestCPPCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


int32 ANetworkingTestCPPCharacter::getPlayerID() 
{
	return GetController()->PlayerState->PlayerId;

}


// set player's action state
void ANetworkingTestCPPCharacter::setPlayerActionState(EPlayerActionState newState)
{
	CharacterStats.PlayerActionState = newState;
}
void ANetworkingTestCPPCharacter::Server_setPlayerActionState_Implementation(EPlayerActionState newState)
{
	setPlayerActionState(newState);
}
bool ANetworkingTestCPPCharacter::Server_setPlayerActionState_Validate(EPlayerActionState newState)
{
	return true;
}

// set player's movement state
void ANetworkingTestCPPCharacter::setPlayerMovementState(EPlayerMovementState newState)
{
	CharacterStats.PlayerMovementState = newState;
}
void ANetworkingTestCPPCharacter::Server_setPlayerMovementState_Implementation(EPlayerMovementState newState)
{
	setPlayerMovementState(newState);
}
bool ANetworkingTestCPPCharacter::Server_setPlayerMovementState_Validate(EPlayerMovementState newState)
{
	return true;
}

// check if player is casting
bool ANetworkingTestCPPCharacter::isCasting()
{
	if (CharacterStats.PlayerActionState == EPlayerActionState::Casting)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// check if player is targeting
bool ANetworkingTestCPPCharacter::isTargeting()
{
	if (CharacterStats.PlayerActionState == EPlayerActionState::Targeting)
	{
		return true;
	}
	else
	{
		return false;
	}
}


// check if player is targeting
bool ANetworkingTestCPPCharacter::isIdle()
{
	if (CharacterStats.PlayerActionState == EPlayerActionState::Idle)
	{
		return true;
	}
	else
	{
		return false;
	}
}


// check if player is disabled
bool ANetworkingTestCPPCharacter::isDisabled()
{
	if (CharacterStats.PlayerMovementState == EPlayerMovementState::Stunned)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void ANetworkingTestCPPCharacter::onAbilityCompleted_Implementation(const AAbility_Master* ability)
{





}

void ANetworkingTestCPPCharacter::onAbilityCancelled_Implementation(const AAbility_Master* ability)
{



}


void ANetworkingTestCPPCharacter::onAbilityTargeting_Implementation(const AAbility_Master* ability)
{



}

void ANetworkingTestCPPCharacter::onCasting_Implementation(FName CastName, float duration, bool bShouldCountUp)
{


}