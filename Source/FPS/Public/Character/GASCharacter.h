// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GASCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class FPS_API AGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Ability System Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	
	// Sets default values for this character's properties
	AGASCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Get AbilitySystemComponent
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// Initialize the Ability System Component on the server
	virtual void PossessedBy(AController* NewController) override;
	
	// Initialize the Ability System Component on the client
	virtual void OnRep_PlayerState() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	EGameplayEffectReplicationMode ASCReplicationMode = EGameplayEffectReplicationMode::Mixed;


private:
	// FP Arms
	UPROPERTY(VisibleAnywhere, Category = "First Person")
	TObjectPtr<USkeletalMeshComponent> FirstPersonArms;
	
	UPROPERTY(VisibleAnywhere, Category = "First Person")
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, Category = "First Person")
	TObjectPtr<UCameraComponent> FirstPersonCamera;
};
