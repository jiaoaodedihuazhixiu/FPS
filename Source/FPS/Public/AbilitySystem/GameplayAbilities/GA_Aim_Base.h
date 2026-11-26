// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GA_Base.h"
#include "Weapon/Weapon.h"
#include "Weapon/Data/WeaponData.h"
#include "Component/Combat.h"
#include "Tags/FPSTags.h"
#include "GA_Aim_Base.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UGA_Aim_Base : public UGA_Base
{
	GENERATED_BODY()
	
public:
	UGA_Aim_Base();
	
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayTagContainer* SourceTags = nullptr, 
		const FGameplayTagContainer* TargetTags = nullptr, 
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
protected:
	virtual void ActivateAbility(	
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;
	
	UFUNCTION()
	void OnInputReleased(float TimeHeld);
	
	UPROPERTY()
	AWeapon* CurrentWeapon;
	
	UPROPERTY()
	UWeaponData* CurrentWeaponData;
	
	AWeapon* ResolveWeapon(const FGameplayAbilityActorInfo* ActorInfo) const;
	UWeaponData* ResolveWeaponData(const FGameplayAbilityActorInfo* ActorInfo) const;
};
