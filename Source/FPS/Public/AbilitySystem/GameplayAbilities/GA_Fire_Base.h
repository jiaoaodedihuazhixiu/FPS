// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GA_Base.h"
#include "Weapon/Weapon.h"
#include "Weapon/Data/WeaponData.h"
#include "GA_Fire_Base.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UGA_Fire_Base : public UGA_Base
{
	GENERATED_BODY()
	
public:
	UGA_Fire_Base();
	
	UFUNCTION(BlueprintCallable, Category="FPS|Fire")
	virtual void FireOnce();

	UFUNCTION(BlueprintCallable, Category="FPS|Fire")
	virtual void StartFire();
	
	UFUNCTION(BlueprintCallable, Category="FPS|Fire")
	virtual void StopFire();
	
	virtual bool CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayTagContainer* SourceTags = nullptr,
	const FGameplayTagContainer* TargetTags = nullptr,
	OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
protected:
	/**
	 * WaitInputRelease 的回调：玩家松开开火键。
	 * TimeHeld = 从 WaitInputRelease 开始等到松手的秒数（蓄力 / 连发计数以后能用上）
	 */
	UFUNCTION()
	virtual void OnInputReleased(float TimeHeld);
	
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
	
	UPROPERTY()
	AWeapon* CurrentWeapon;
	
	UPROPERTY()
	UWeaponData* CurrentWeaponData;
	
	AWeapon* ResolveWeapon(const FGameplayAbilityActorInfo* ActorInfo) const;
	UWeaponData* ResolveWeaponData(const FGameplayAbilityActorInfo* ActorInfo) const;
};


