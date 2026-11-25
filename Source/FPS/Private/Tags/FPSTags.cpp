// Copyright Druid Mechanics

#include "Tags/FPSTags.h"

/*
 * 三条硬规则：
 *   1. UE_DEFINE_GAMEPLAY_TAG 只能写在 .cpp 里（宏内部有 static_assert 检查文件扩展名）。
 *      要在别的文件里用，就在 .h 里 UE_DECLARE_GAMEPLAY_TAG_EXTERN，在 .cpp 里 UE_DEFINE_GAMEPLAY_TAG。
 *   2. 第三个参数是编辑器里显示的开发者注释，会走 TEXT()，保持 ASCII。
 *   3. tag 字符串一旦发布就别改 —— 它是存档、网络、蓝图资产引用的身份。
 */
namespace FPSTags
{
	// ------------------------------------------------------------------ 能力身份
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Weapon_Fire,    "Ability.Weapon.Fire",      "Fire weapon ability");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Weapon_Aim,     "Ability.Weapon.Aim",       "Aim down sights");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Movement_Dash,  "Ability.Movement.Dash",    "Dash");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Heal,           "Ability.Heal",             "Heal");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_SpawnBlock,     "Ability.SpawnBlock",       "Spawn block");

	// ------------------------------------------------------------------ 状态
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Firing,    "State.Firing",    "Owner is currently firing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Aiming,    "State.Aiming",    "Owner is aiming");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Dashing,   "State.Dashing",   "Owner is dashing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Reloading, "State.Reloading", "Owner is reloading");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Dead,      "State.Dead",      "Owner is dead");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Healing,   "State.Healing",   "Owner is being healed");

	// ------------------------------------------------------------------ 事件
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Weapon_Fire,    "Event.Weapon.Fire",    "Fire montage notify");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Weapon_DryFire, "Event.Weapon.DryFire", "Out of ammo, dry fire notify");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Weapon_Reload,  "Event.Weapon.Reload",  "Reload finished notify");

	// ------------------------------------------------------------------ SetByCaller
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Data_Damage,   "Data.Damage",   "Damage amount for SetByCaller");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Data_AmmoCost, "Data.AmmoCost", "Ammo cost for SetByCaller");

	// ------------------------------------------------------------------ 冷却
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown_Dash,       "Cooldown.Dash",       "Dash cooldown");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown_Heal,       "Cooldown.Heal",       "Heal cooldown");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown_SpawnBlock, "Cooldown.SpawnBlock", "Spawn block cooldown");
}
