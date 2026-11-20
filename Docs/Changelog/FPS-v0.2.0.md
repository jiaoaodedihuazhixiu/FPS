# FPS-v0.2.0（2026.11.20）

> **本版本范围**：放弃 v0.1.x 的全蓝图 GA 方案，确立「C++ 主干 + 蓝图表现」的混合架构。搭建 `AbilitySystem` 源码目录骨架（AttributeSet + 能力基类），新增 `UCombat` 组件作为战斗输入统一入口，战斗输入绑定从 `AGASPlayerController` 迁移到 `AGASCharacter`。蓝图开火能力族（`GA_FireModeBase` / `GA_SemiAuto` / `GA_FullAuto` / `GA_Burst`）、`GA_Dash`、`WeaponManageConponent` 全部删除。开火链路在本版本**不可用**，属于以功能换架构的重构版本。下一步：补内容到动画蓝图（`ABP_FirstPerson` / `ABP_ThirdPerson`）。

## 源码（`Source/FPS`）

| 类 | 文件 | 变更 |
| --- | --- | --- |
| `UFPSAttributeSet` | `AbilitySystem/AttributeSets/FPSAttributeSet.h` / `.cpp` | **新增**。属性：`Health` / `MaxHealth` / `MoveSpeed` / `MaxMoveSpeed`（构造函数默认 100 / 100 / 600 / 600），全部 `ReplicatedUsing` + `GetLifetimeReplicatedProps`（`COND_None` + `REPNOTIFY_Always`）。弹药属性经评估后**不放入 AttributeSet**，采用方案 A：弹药归属武器（见「本版本边界」第 3 条） |
| `UGA_Base` | `AbilitySystem/GameplayAbilities/GA_Base.h` / `.cpp` | 由 `UGASGameplayAbilityBase`（`GameplayAbility/` 目录）迁移重命名，内容不变（仅 `bShouldShowOnHUD`），作为全部能力基类 |
| `UGA_Fire_Base` | `AbilitySystem/GameplayAbilities/GA_Fire_Base.h` / `.cpp` | **新增**，继承 `UGA_Base`，开火能力 C++ 基类。本版本为空壳，弹道（相机判定 + 枪口表现）、伤害 GE、预测在下个版本实现 |
| `UCombat` | `Component/Combat.h` / `.cpp` | **新增** ActorComponent（`SetIsReplicated(true)`），战斗输入统一入口：`Initiate_CycleWeapon` / `Initiate_Aim` / `Initiate_Fire_Pressed` / `Initiate_Fire_Released`，当前均为调试输出桩，未接 GA |
| `AGASCharacter` | `Character/GASCharacter.h` / `.cpp` | 新增 `CombatComponent` 子对象；战斗输入（`IA_Fire` / `IA_Aim` / `IA_CycleWeapon`）绑定到 `SetupPlayerInputComponent` 并转发给 `UCombat`；移除 `CurrentWeapon` / `SetCurrentWeapon`（待新武器系统重建，重建时带复制） |
| `AWeapon` | `Weapon/Weapon.h` / `.cpp` | 网格 Getter 改为头文件内联 `BlueprintPure`，无行为变化 |
| `UGASGameplayAbilityBase`、`UGA_FireBase`（旧） | `GameplayAbility/` 目录 | **删除**，由 `AbilitySystem/GameplayAbilities/` 下的新类替代 |

## GA 清单与类型（本版本时点）

| GA | 类型 | 状态 / 规划 |
| --- | --- | --- |
| `UGA_Base` | **C++** | 全部能力基类 |
| `UGA_Fire_Base` | **C++** | 开火基类，空壳待实现（高频 + 预测敏感 + 弹道判定，必须 C++） |
| `GA_Aim` | 蓝图 | 保留占位；规划下沉 C++（状态型 + GE 生命周期管理） |
| `GA_Heal` | 蓝图 | 保留（低频、纯 GE 应用，蓝图足够） |
| `GA_SpawnBlock` | 蓝图 | 保留（低频、spawn 走服务端即可） |
| `GA_TestAbility` | 蓝图 | 测试用，交付前删除 |
| `GA_FireModeBase` / `GA_SemiAuto` / `GA_FullAuto` / `GA_Burst` | ~~蓝图~~ | **已删除**，由 `UGA_Fire_Base` 的 C++ 体系替代 |
| `GA_Dash` | ~~蓝图~~ | **已删除**；规划以 C++ 重建（Root Motion + 预测回滚，蓝图是重灾区） |
| `GA_Reload` | 未创建 | 规划蓝图（Montage 流程能力，与开火的互斥走 GameplayTag 数据） |
| `GA_Death` | 未创建 | 规划蓝图表现流程 + C++ `PostGameplayEffectExecute` 触发（参照 Lyra `GA_Death` 模式） |

## 资产（`Content`）

| 路径 | 类型 | 说明 |
| --- | --- | --- |
| `GAS/Abilities/Fire/`（整个目录） | 蓝图 | **删除**：`GA_FireModeBase` / `GA_SemiAuto` / `GA_FullAuto` / `GA_Burst` |
| `GAS/Abilities/GA_Dash` | 蓝图 | **删除**，待 C++ 重建 |
| `GAS/Components/WeaponManageConponent` | 蓝图 | **删除**，武器管理职能由 `UCombat`（C++）接管 |
| `GAS/Character/BP_GASCharacter` | 蓝图 | 移除已删组件与能力授予逻辑，清理对已删 GA 的 `None` 引用（修复 `GiveAbility` 空类 ensure 崩溃） |
| `GAS/Character/Animation/ABP_FirstPerson` | 动画蓝图 | 修改（本版本仅骨架调整，**下一步补内容**） |
| `GAS/Character/Animation/ABP_ThirdPerson` | 动画蓝图 | **新增**（**下一步补内容**） |
| `GAS/Input/InputActions/Combat/` | 输入动作 | **新增目录**：`IA_Fire` 等战斗输入动作移入并按职能分组 |
| `GAS/Input/Mapping/IMC_GASInput` | 输入映射 | 随输入目录重组更新 |
| `GAS/Weapons/Pistol/DA_Pistol`、`GAS/Weapons/Rifle/DA_Rifle` | 数据资产 | 清理 `AbilitiesToGrant` 中对已删蓝图能力的失效引用 |
| `Config/DefaultGameplayTags.ini` | 配置 | 标签随能力体系调整 |
| `Config/DefaultEngine.ini` | 配置 | 随重构更新 |

## 本版本边界（据资产与配置核对）

1. **开火链路不可用**：蓝图开火族已删、`UGA_Fire_Base` 为空壳、`UCombat::Initiate_Fire_*` 只有调试输出——本版本无法射击，属于重构中间态。
2. **`UCombat` 是桩**：四个 `Initiate_*` 入口仅 `AddOnScreenDebugMessage`，尚未与 ASC / GA 接通。
3. **弹药方案已定未落地**：方案 A（弹药归属武器）已定——`UWeaponData` 将加 `MagazineSize` / `MaxReserveAmmo`（静态配置），`AWeapon` 将加 `CurrentMagAmmo` / `CurrentReserveAmmo`（replicated 运行状态）；本版本仅完成 AttributeSet 侧的弹药移除。
4. **`UFPSAttributeSet` 缺结算逻辑**：`PostGameplayEffectExecute`（血量 clamp + 死亡检测 → 激活 `GA_Death`）与 `PreAttributeChange` 尚未实现，等伤害 GE 落地时一并编写。
5. **武器持有状态未重建**：`CurrentWeapon` 已随旧逻辑移除，新武器系统需带 `ReplicatedUsing` 复制（服务端唯一赋值）。
6. **能力授予路径缺失**：`UWeaponData::AbilitiesToGrant` 字段保留，但 C++ 侧尚无调用 `GiveAbility` 的授予逻辑（旧蓝图授予已随 `BP_GASCharacter` 清理移除），且授予前需加空类保护（本次 ensure 崩溃的教训）。
7. **下一步**：补内容到动画蓝图（`ABP_FirstPerson` / `ABP_ThirdPerson`）。

## 参考文档

- [C++ 类结构图（FPS-v0.2.0 时点）](../Notes/Cpp-Class-Structure.md)
