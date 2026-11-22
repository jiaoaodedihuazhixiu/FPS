// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "InputAction.h"
#include "Component/Combat.h"
#include "GameFramework/Character.h"
#include "Interface/PlayerInterface.h"
#include "Weapon/Weapon.h"
#include "GASCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class FPS_API AGASCharacter : public ACharacter, public IAbilitySystemInterface, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AGASCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UCombat* CombatComponent;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
	
	/* From IPlayerInterface */
	virtual USkeletalMeshComponent* GetFirstPersonArms_Implementation() const override;
	virtual USkeletalMeshComponent* GetThirdPersonMesh_Implementation() const override;
	
	virtual void BeginDestroy() override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	EGameplayEffectReplicationMode ASCReplicationMode = EGameplayEffectReplicationMode::Mixed;

private:
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
