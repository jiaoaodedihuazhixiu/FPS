// Copyright Druid Mechanics

#pragma once

#include "NativeGameplayTags.h"

/**
 * 本项目所有「C++ 里要引用」的 GameplayTag：在这里声明，在 FPSTags.cpp 里定义。
 *
 * 为什么不写在 DefaultGameplayTags.ini：
 *   native tag 是模块静态构造期注册的，早于任何蓝图 CDO 构造，所以能安全地用在构造函数里；
 *   而且它在 C++ 里是一个真实变量，拼错是编译错误，不是运行时的空 tag。
 *
 * 用法：
 *   #include "Tags/FPSTags.h"
 *
 *   // 构造函数里
 *   ActivationOwnedTags.AddTag(FPSTags::State_Firing.GetTag());
 *
 *   // 运行时判断
 *   ASC->HasMatchingGameplayTag(FPSTags::State_Firing.GetTag());
 *
 * 哪些 tag 应该留在 ini 里（不要迁）：
 *   Weapon.Type.None / Pistol / Rifle —— 给策划在武器蓝图的 WeaponType 属性里挑，C++ 不引用。
 * 约定：
 *   Ability.*  能力身份  → SetAssetTags()
 *   State.*    状态      → ActivationOwnedTags / ActivationBlockedTags
 *   Event.*    事件      → SendGameplayEventToActor / WaitGameplayEvent
 *   Data.*     数值键    → GE 的 SetByCaller
 *   Cooldown.* 冷却      → 冷却 GE 的 GrantedTags
 */
namespace FPSTags
{
	// ------------------------------------------------------------------ 能力身份
	// 挂在 FGameplayAbilitySpec 上当身份用，可以被 ASC->TryActivateAbilitiesByTag 命中
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Weapon_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Weapon_Aim);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Dash);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Heal);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_SpawnBlock);

	// ------------------------------------------------------------------ 状态
	// 用 ActivationOwnedTags 挂到 owner 身上：激活期间存在，能力结束自动摘掉
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Firing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Aiming);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dashing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Reloading);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Healing);

	// ------------------------------------------------------------------ 事件
	// AnimNotify 里 SendGameplayEventToActor 发，能力里 WaitGameplayEvent 收
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Weapon_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Weapon_DryFire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Weapon_Reload);

	// ------------------------------------------------------------------ SetByCaller
	// GE 里 modifier 的 SetByCaller 必须填同名 tag，否则 apply 时会 ensure "SetByCaller Tag not found"
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_AmmoCost);

	// ------------------------------------------------------------------ 冷却
	// 冷却 GE 的 GrantedTags 填这里，CheckCooldown 靠它判断（开火靠射速，不用 cooldown）
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Dash);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Heal);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_SpawnBlock);
}
