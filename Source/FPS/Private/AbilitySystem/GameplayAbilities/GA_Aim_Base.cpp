// Copyright Druid Mechanics


#include "AbilitySystem/GameplayAbilities/GA_Aim_Base.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Character/GASCharacter.h"


UGA_Aim_Base::UGA_Aim_Base()
{
	/* Defaults */
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	
	/* Tags */
	SetAssetTags(FGameplayTagContainer(FPSTags::Ability_Weapon_Aim.GetTag()));

	ActivationOwnedTags.AddTag(FPSTags::State_Aiming);
	
	ActivationBlockedTags.AddTag(FPSTags::State_Dashing);
	ActivationBlockedTags.AddTag(FPSTags::State_Dead);
}

bool UGA_Aim_Base::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	if (!ActorInfo)
	{
		return false;
	}
	
	return true;
}

void UGA_Aim_Base::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AWeapon* Weapon = ResolveWeapon(ActorInfo);
	UWeaponData* WeaponData = ResolveWeaponData(ActorInfo);
	
	if (!WeaponData || !Weapon)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	CurrentWeapon = Weapon;
	CurrentWeaponData = WeaponData;
	
	if (!IsActive())
	{
		return;
	}
	
	AActor* Avatar = ActorInfo->AvatarActor.Get();
	UCombat* Combat = Avatar->GetComponentByClass<UCombat>();
	Combat->SetIsAiming(true);
	AGASCharacter* Character = Cast<AGASCharacter>(ActorInfo->AvatarActor.Get());
	Character->OnAim(Combat->bIsAiming);
	
	UAbilityTask_WaitInputRelease* ReleaseTask =
		UAbilityTask_WaitInputRelease::WaitInputRelease(this, /*bTestAlreadyReleased=*/true);
	
	if (ReleaseTask)
	{
		ReleaseTask->OnRelease.AddDynamic(this, &UGA_Aim_Base::OnInputReleased);
		ReleaseTask->ReadyForActivation();
	}
}

void UGA_Aim_Base::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, 
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	AActor* Avatar = ActorInfo->AvatarActor.Get();
	UCombat* Combat = Avatar->GetComponentByClass<UCombat>();
	Combat->SetIsAiming(false);
	AGASCharacter* Character = Cast<AGASCharacter>(ActorInfo->AvatarActor.Get());
	Character->OnAim(Combat->bIsAiming);
}

void UGA_Aim_Base::OnInputReleased(float TimeHeld)
{
	if (IsActive())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
				   /*bReplicateEndAbility*/ true,
				   /*bWasCancelled*/ false);
	}
}

AWeapon* UGA_Aim_Base::ResolveWeapon(const FGameplayAbilityActorInfo* ActorInfo) const
{
	if (!ActorInfo) { return nullptr; }
	
	AActor* Avatar = ActorInfo->AvatarActor.Get();
	if (!Avatar) { return nullptr; }

	UCombat* Combat = Avatar->GetComponentByClass<UCombat>();
	return Combat ? Combat->GetCurrentWeapon() : nullptr;
}

UWeaponData* UGA_Aim_Base::ResolveWeaponData(const FGameplayAbilityActorInfo* ActorInfo) const
{
	const AWeapon* Weapon = ResolveWeapon(ActorInfo);
	return Weapon ? Weapon->WeaponData : nullptr;
}
