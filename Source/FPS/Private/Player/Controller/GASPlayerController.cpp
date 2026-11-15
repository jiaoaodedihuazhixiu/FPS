// Copyright Druid Mechanics


#include "Player/Controller/GASPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Character/GASCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Constructor
AGASPlayerController::AGASPlayerController()
{
	bReplicates = true;
	
}

void AGASPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem;
	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	if (IsValid(Subsystem))
	{
		Subsystem->AddMappingContext(GASIMC, 0);
	}
}

void AGASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* GASInputComponent = CastChecked<UEnhancedInputComponent, UInputComponent>(InputComponent);
	GASInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGASPlayerController::Input_Move);
	GASInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGASPlayerController::Input_Look);
	GASInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AGASPlayerController::Input_Jump);
	GASInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AGASPlayerController::Input_Crouch);
}

void AGASPlayerController::Input_Jump()
{
	if (!IsValid(GetCharacter()))	return;
	
	UCharacterMovementComponent* CMC = GetCharacter()->GetCharacterMovement();
	if (!IsValid(CMC))	return;

	if (CMC->bWantsToCrouch)
	{
		CMC->bWantsToCrouch = !CMC->bWantsToCrouch;
	}
	else
	{
		GetCharacter()->Jump();
	}
	
}

void AGASPlayerController::Input_Crouch()
{
	if (!IsValid(GetCharacter()))	return;
	
	UCharacterMovementComponent* CMC = GetCharacter()->GetCharacterMovement();
	if (!IsValid(CMC))	return;
	
	if (IsValid(CMC))
	{
		CMC->bWantsToCrouch = !CMC->bWantsToCrouch;
	}
}

void AGASPlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	
	AddYawInput(InputAxisVector.X);
	AddPitchInput(InputAxisVector.Y);
}

void AGASPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

