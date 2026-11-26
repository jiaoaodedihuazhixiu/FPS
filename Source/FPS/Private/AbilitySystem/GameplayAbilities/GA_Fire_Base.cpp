// Copyright Druid Mechanics


#include "AbilitySystem/GameplayAbilities/GA_Fire_Base.h"

#include "KismetTraceUtils.h"
#include "Abilities/GameplayAbilityTargetActor_SingleLineTrace.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Component/Combat.h"
#include "Tags/FPSTags.h"
#include "Weapon/Weapon.h"

UGA_Fire_Base::UGA_Fire_Base()
{
	/* Defaults */
	InstancingPolicy   = EGameplayAbilityInstancingPolicy::InstancedPerActor;      
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;       
	ReplicationPolicy  = EGameplayAbilityReplicationPolicy::ReplicateNo;          
	
	/* Tags */
	SetAssetTags(FGameplayTagContainer(FPSTags::Ability_Weapon_Fire.GetTag()));
	
	ActivationOwnedTags.AddTag(FPSTags::State_Firing.GetTag());
	
	ActivationBlockedTags.AddTag(FPSTags::State_Dashing.GetTag());
	ActivationBlockedTags.AddTag(FPSTags::State_Dead.GetTag());
	ActivationBlockedTags.AddTag(FPSTags::State_Reloading.GetTag());
}

bool UGA_Fire_Base::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const AWeapon* Weapon = ResolveWeapon(ActorInfo);
	if (!Weapon) { return false; }

	const UWeaponData* WeaponData = Weapon->WeaponData;
	if (!WeaponData) { return false; }

	// 如果以后按枪种拆能力，这里比对 WeaponType（现在 AWeapon::WeaponType 是 protected 没 getter）

	return true;
}

void UGA_Fire_Base::ActivateAbility(
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
	
	// 先挂监听再开火：StartFire 里若因弹药耗尽直接 EndAbility，
	// 任务会随能力一起回收，不会留下"能力已结束、任务还在等"的孤儿。
	StartFire();
	if (!IsActive())
	{
		return;
	}
	
	UAbilityTask_WaitInputRelease* ReleaseTask =
		UAbilityTask_WaitInputRelease::WaitInputRelease(this, /*bTestAlreadyReleased=*/true);
	
	/* 
	 * WaitReleased
	 * 这个任务等的不是 Enhanced Input 的 Completed，而是 ASC 的通用复制事件 InputReleased。
	 * 唯一合法的触发路径是：ASC->AbilityLocalInputReleased(InputID)
	 *     AbilityLocalInputReleased
	 *         → InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased)
	 *         → 本任务注册在 AbilityReplicatedEventDelegate 上的 OnReleaseCallback
	 *         → OnRelease(TimeHeld)
	 * 所以 GA 的 spec 必须带 InputID，且按键要经 ASC 转发（见 UCombat::Initiate_Fire_*）。
	 * 靠蓝图 TryActivateAbility / SendGameplayEvent 激活的话，这条链根本不会被踩到。
	 *
	 * bTestAlreadyReleased = true：激活瞬间若 Spec->InputPressed 已经是 false
	 * （说明这次不是"按住"触发的），任务立刻回调，效果退化成"打一发就结束"，
	 * 而不是把 State.Firing 卡在身上等一个永远不来的松手。
	 */
	if (ReleaseTask)
	{
		ReleaseTask->OnRelease.AddDynamic(this, &UGA_Fire_Base::OnInputReleased);
		ReleaseTask->ReadyForActivation();
	}
}

void UGA_Fire_Base::OnInputReleased(float TimeHeld)
{
	// 松手 = 停火。TimeHeld 目前没用，留给蓄力 / 三连发计数。
	StopFire();
}

void UGA_Fire_Base::EndAbility(const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled)
{
	Super::EndAbility(
		Handle, 
		ActorInfo, 
		ActivationInfo, 
		bReplicateEndAbility, 
		bWasCancelled);
}

void UGA_Fire_Base::FireOnce()
{
	// 实现顺序是倒着来的
	// ①
	// 弹药不够 → StopFire() + EndAbility，直接 return
	// CommitAbilityCost()
	// CommitAbility 会附带 ApplyCooldown
	
	
	// ② 
	// PlayMontageAndWait
	
	
	// ③ 
	// 先做服务器自算，后面再改成TargetData
	// HasAuthority：服务器校验 + SpawnActor(子弹) + 把伤害Spec交给子弹
	if (!HasAuthority(&CurrentActivationInfo))
	{
		return;
	}
	
	FHitResult OutHit;
	CurrentWeapon->WeaponTrace(OutHit);
}

void UGA_Fire_Base::StartFire()
{
	const bool bAuthority = K2_HasAuthority();
	GEngine->AddOnScreenDebugMessage(-1, 3.f,
		bAuthority ? FColor::Red : FColor::Green,
		bAuthority ? TEXT("StartFire: 服务器") : TEXT("StartFire: 客户端"));
	FireOnce();
}

void UGA_Fire_Base::StopFire()
{
	/* 松手时能力可能已经被别的原因结束了（换弹 Cancel、死亡、弹药耗尽）。
	 * 重复 EndAbility 会让 ActivationOwnedTags / BlockedAbilityTags 的计数漂移，
	 * 表现成"有时候能挡、有时候挡不住"，所以先判活。 */
	if (!IsActive()) { return; }
	
	const bool bAuthority = K2_HasAuthority();
	GEngine->AddOnScreenDebugMessage(-1, 3.f,
		bAuthority ? FColor::Red : FColor::Green,
		bAuthority ? TEXT("StopFire: 服务器") : TEXT("StopFire: 客户端"));
	
	EndAbility(CurrentSpecHandle, 
		CurrentActorInfo, 
		CurrentActivationInfo, 
		true, 
		false);
}


AWeapon* UGA_Fire_Base::ResolveWeapon(const FGameplayAbilityActorInfo* ActorInfo) const
{
	if (!ActorInfo) { return nullptr; }
	
	AActor* Avatar = ActorInfo->AvatarActor.Get();
	if (!Avatar) { return nullptr; }

	UCombat* Combat = Avatar->FindComponentByClass<UCombat>();
	return Combat ? Combat->GetCurrentWeapon() : nullptr;
}

UWeaponData* UGA_Fire_Base::ResolveWeaponData(const FGameplayAbilityActorInfo* ActorInfo) const
{
	const AWeapon* Weapon = ResolveWeapon(ActorInfo);
	return Weapon ? Weapon->WeaponData : nullptr;
}
