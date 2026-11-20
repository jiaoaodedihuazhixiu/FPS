// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "InputAction.h"
#include "Component/Combat.h"
#include "GameFramework/Character.h"
#include "Weapon/Weapon.h"
#include "GASCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class FPS_API AGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	
	UFUNCTION(BlueprintPure)
	USkeletalMeshComponent* GetFirstPersonArms() const { return FirstPersonArms; }
	
	UFUNCTION(BlueprintPure)
	UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCamera; }

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	EGameplayEffectReplicationMode ASCReplicationMode = EGameplayEffectReplicationMode::Mixed;

	
private:
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UCombat* CombatComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "First Person")
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, Category = "First Person")
	TObjectPtr<UCameraComponent> FirstPersonCamera;
	
	UPROPERTY(VisibleAnywhere, Category = "First Person")
	TObjectPtr<USkeletalMeshComponent> FirstPersonArms;
	
	// Combat Inputs
	UPROPERTY(EditAnywhere, Category = "FPS|Input")
	TObjectPtr<UInputAction> IA_CycleWeapon;
	
	UPROPERTY(EditAnywhere, Category = "FPS|Input")
	TObjectPtr<UInputAction> IA_Fire;
	
	UPROPERTY(EditAnywhere, Category = "FPS|Input")
	TObjectPtr<UInputAction> IA_Aim;
	
	/*  Functions  */
	void Input_CycleWeapon();
	void Input_Aim();
	void Input_Fire_Pressed();
	void Input_Fire_Released();
};
