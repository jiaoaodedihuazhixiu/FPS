# 版本记录

> **项目**：FPS（`D:\Unreal Engine Project\FPS`）
> **引擎**：UE 5.8（引擎目录 `D:\Unreal Engine\UE_5.8`）
> **版本格式约定**：标签统一写作 `FPS-v<主>.<次>.<修订>`，例如 `FPS-v0.1.0`。标签指向**含该版本说明的提交**——`git reset --hard FPS-v0.1.0` 回退后，这个版本的说明也会一并回来。
> **记录约定**：每个版本一个一级标题，新版本加在最上面；版本内容分「源码」与「资产」两类列清，并写明该版本的边界（哪些还没做）。

---

# FPS-v0.1.0（2026.11.15）

> **本版本范围**：只有角色、武器的基础设置，武器的数据类，角色控制器类，以及游戏能力类。场景搭建、玩法循环、伤害结算、UI 均未进入本版本。

## 源码（`Source/FPS`）

头文件在 `Public/`，实现文件在 `Private/`，两边同构（下表的路径省略重复前缀）。

| 类 | 文件 | 职责 |
| --- | --- | --- |
| `AGASCharacter` | `Character/GASCharacter.h` / `.cpp` | 继承 `ACharacter`。第一人称三件套：`FirstPersonArms`（`USkeletalMeshComponent`）、`SpringArm`（`USpringArmComponent`）、`FirstPersonCamera`（`UCameraComponent`），均 `VisibleAnywhere`。重写 `BeginPlay` / `Tick` / `SetupPlayerInputComponent` |
| `AGASPlayerController` | `Player/Controller/GASPlayerController.h` / `.cpp` | 继承 `APlayerController`。增强输入：`GASIMC`（`UInputMappingContext`）+ `Look` / `Jump` / `Crouch` / `Move` 四个 `UInputAction`，均 `EditAnywhere`（`FPS\|Input`）。回调 `Input_Look` / `Input_Move` / `Input_Jump` / `Input_Crouch` |
| `AWeapon` | `Weapon/Weapon.h` / `.cpp` | 继承 `AActor`。`WeaponData`（`UWeaponData*`，`EditAnywhere` + `BlueprintReadOnly`）、`WeaponType`（`FGameplayTag`，protected）。FP / TP 两个 `USkeletalMeshComponent`，对外暴露 `GetFirstPersonMesh()` / `GetThirdPersonMesh()`（`BlueprintCallable`） |
| `UWeaponData` | `Weapon/Data/WeaponData.h` / `.cpp` | 继承 `UDataAsset`。当前唯一数据是 `TMap<FGameplayTag, FName> GripPoints`（`EditDefaultsOnly`）。选 `UDataAsset` 而非 DataTable：蓝图里从 `UWeaponData*` 引脚直接取属性，比 `Get Data Table Row → Break Struct` 少一个节点，且引用本身就是武器身份 |
| `UGASGameplayAbilityBase` | `GameplayAbility/GASGameplayAbilityBase.h` / `.cpp` | 继承 `UGameplayAbility`。`bShouldShowOnHUD`（`EditAnywhere`，默认 `false`），用作 HUD 是否显示该技能 |
| `UGA_FireBase` | `GameplayAbility/GA_FireBase.h` / `.cpp` | 继承 `UGASGameplayAbilityBase`。`StartFire` / `StopFire` / `FireOnce` 三个 `BlueprintImplementableEvent`，**C++ 侧无实现**，逻辑全在蓝图 |
| `AGASGamemodeBase` | `Gamemode/GASGamemodeBase.h` / `.cpp` | 继承 `AGameModeBase`，当前是空壳（仅类声明） |
| 模块与构建 | `FPS.Build.cs`、`FPS.cpp` / `FPS.h`、`FPS.Target.cs`、`FPSEditor.Target.cs` | 模块入口，以及 Game / Editor 两个 Target |

## 资产（`Content/GAS`）

| 路径 | 类型 | 对应实现 |
| --- | --- | --- |
| `Character/BP_GASCharacter` | 蓝图（Character） | `AGASCharacter` |
| `Player/BP_GASPlayerController` | 蓝图（PlayerController） | `AGASPlayerController` |
| `Gamemode/BP_GASGamemodeBase` | 蓝图（GameModeBase） | `AGASGamemodeBase` |
| `Weapons/BP_WeaponBase` | 蓝图（Actor） | `AWeapon` 基类 |
| `Weapons/Pistol/BP_Pistol`、`Weapons/Rifle/BP_Rifle` | 蓝图 | 手枪 / 步枪武器实例（FP + TP 双网格） |
| `Weapons/Pistol/DA_Pistol`、`Weapons/Rifle/DA_Rifle` | 数据资产（`UWeaponData`） | 两把枪的 `GripPoints` 配置 |
| `Abilities/GA_FireModeBase` | 蓝图（`UGA_FireBase`） | 开火基类 |
| `Abilities/GA_Dash`、`Abilities/GA_Heal`、`Abilities/GA_SpawnBlock` | 蓝图（`UGASGameplayAbilityBase`） | 冲刺 / 治疗 / 生成障碍 |
| `Input/InputActions/IA_Move`、`IA_Look`、`IA_Jump`、`IA_Crouch` | `UInputAction` | 四个输入动作 |
| `Input/Mapping/IMC_GASInput` | `UInputMappingContext` | 输入映射表 |

## 本版本边界（据源码核对）

1. **`AGASCharacter` 还没有 ASC**：未实现 `IAbilitySystemInterface`，也没有 `UAbilitySystemComponent` 成员。能力蓝图当前无法在角色侧直接挂载。
2. **`UGA_FireBase` 的 C++ 侧是空的**：三个事件都是 `BlueprintImplementableEvent`，开火时序完全由蓝图决定。
3. **`UWeaponData` 字段极简**：只有 `GripPoints`，没有射速、伤害、弹药、冷却等数值字段。
4. **`AGASGamemodeBase` 为空壳**：尚未设置默认 Pawn / PlayerController 等默认类。

---

# 附：分清 `AbilityTags` 与 `ActivationOwnedTags`

> 从 2026.11.10 的记录中保留。这两个容器混用是标签体系最常见的错源，且引擎不报错、只是静默不生效。

## 前提：两类容器用途完全不同（源码依据）

分配标签之前必须先分清 `AbilityTags` 和 `ActivationOwnedTags`，因为**引擎在取消/阻断时只认前者**：

- `UGameplayAbility::GetAssetTags()` 直接 `return AbilityTags;`（`GameplayAbility.cpp:1299-1302`）
- `UAbilitySystemComponent::CancelAbilities` 拿 `Spec.Ability->GetAssetTags()` 去比对（`AbilitySystemComponent_Abilities.cpp:1339-1341`）
- `BlockAbilitiesWithTag` / `CancelAbilitiesWithTag` 同样走 `GetAssetTags()`（`GameplayAbility.cpp:736`、`:888`）

**推论：`ActivationOwnedTags` 只负责"挂状态"，不参与任何取消与阻断的匹配。** 想让 `GA_Heal` 能被中断，`GameplayAbility.Heal` 必须放在它的 `AbilityTags` 里。

| 容器 | 装什么 | 谁用它 |
| --- | --- | --- |
| `AbilityTags`（身份） | `GameplayAbility.Fire` / `.Dash` / `.Heal` / `.SpawnBlock` | `CancelAbilities`、`BlockAbilitiesWithTag`、`CancelAbilitiesWithTag` |
| `ActivationOwnedTags`（运行状态） | `State.Fire` / `State.Dashing` / `State.Healing` | 其他 GA 的 `ActivationBlockedTags`、UI、动画 |
| 冷却 GE 的 `GrantedTags` | `Cooldown.Fire` / `.Dash` / `.Heal` / `.SpawnBlock` | 引擎 `CheckCooldown`（自动，不用写进 `ActivationBlockedTags`） |
| 运行时 GE 的 `GrantedTags`（角色状态） | `State.Dead` | 所有 GA 的 `ActivationBlockedTags` |

**命名铁律**：`GameplayAbility.*` 只进 `AbilityTags`；`State.*` 只进 `ActivationOwnedTags` 或 GE 的 `GrantedTags`；`Cooldown.*` 只由冷却 GE 授予。两个根不混用，看到前缀就知道该填哪个容器。

## 路径 1 / 路径 2：`ActivationBlockedTags` 与"用标签阻止能力"

两个机制**匹配的对象完全不同**，这是"用标签阻止能力"看起来不生效的根源。`DoesAbilitySatisfyTagRequirements` 里连续两次 `CheckForBlocked`（`GameplayAbility.cpp:406-408`）：

```cpp
// 第 1 条：BlockAbilitiesWithTag 的路径
CheckForBlocked(GetAssetTags(), AbilitySystemComponent.GetBlockedAbilityTags());

// 第 2 条：ActivationBlockedTags 的路径
CheckForBlocked(AbilitySystemComponent.GetOwnedGameplayTags(), ActivationBlockedTags);
```

| | 左边（我有什么） | 右边（什么挡我） |
| --- | --- | --- |
| **路径 1** `BlockAbilitiesWithTag` | 我的 **`AbilityTags`**（能力身份） | ASC 的 `BlockedAbilityTags`（由**别人**的 `BlockAbilitiesWithTag` 填进去） |
| **路径 2** `ActivationBlockedTags` | **ASC 当前拥有的所有标签** | 我在 GA 上写的 `ActivationBlockedTags` |

> 路径 1 只认 `AbilityTags`；`ActivationOwnedTags` 完全不参与。
> 路径 2 认 ASC 的 owned tags——`ActivationOwnedTags`、GE 的 `GrantedTags`、loose tag 全部算。

按发生概率排列的五个失败原因：

1. **把 `State.*` 写进了 `BlockAbilitiesWithTag`。**
   `BlockAbilitiesWithTag` 会被拿去和**对方的 `AbilityTags`** 比对。若开火 GA 的 `AbilityTags = GameplayAbility.Fire`，那么写 `State.Dashing` 永远匹配不上——**不报错、不警告，安静地不生效**。
2. **对方的 GA 没填 `AbilityTags`。**
   `GetAssetTags()` 直接返回 `AbilityTags`，为空则 `HasAny` 恒为 false，任何 `BlockAbilitiesWithTag` 都无效。教程通常只教填 `ActivationOwnedTags`，"Ability Tags"那一栏常常空着。
   （排除一个常见误解：**阻断不需要额外勾开关**。激活时 `bIsBlockingOtherAbilities` 被强制置 true（`GameplayAbility.cpp:966`），并以 `bEnableBlockTags = true` 调用 `ApplyAbilityBlockAndCancelTags`（`:999`）；`EndAbility` 时以 `false` 调一次自动解除（`:888`）。只要 `BlockAbilitiesWithTag` 非空就一定生效，问题只可能出在标签匹配上。）
3. **阻断只在"阻断者处于激活状态"期间有效。**
   `BlockAbilitiesWithTag` 是在阻断者激活时把标签加进 ASC 的 `BlockedAbilityTags`、结束时再减掉。所以阻断者没激活、被提前 cancel 都会让阻断提前消失；更根本的是，**如果状态是 GE 给的（死亡、眩晕、减速），就没有任何能力可以承载 `BlockAbilitiesWithTag`**。
4. **只挡新激活，不停已在跑的能力。**
   两条路径都只影响"能不能激活"。一支持续扫射的全自动已经在跑，加什么标签都不会让它停——这是两者共有的限制，但配合第 3 点很容易误判成"阻断失效"。
5. **`BlockedAbilityTags` 计数漂移。**
   它是 tag count：激活 +1、结束 −1。能力被意外结束或重复结束，count 可能不归零，表现为"有时候能挡，有时候挡不住"。这类 bug 最难查。

## 四个技能的标签分配表

| 技能 | `AbilityTags` | `ActivationOwnedTags` | `ActivationBlockedTags` | 冷却 |
| --- | --- | --- | --- | --- |
| `GA_Fire`（全自动/半自动/三连发共用） | `GameplayAbility.Fire` | `State.Fire` | `State.Dead`、`State.Realoading` | 半自动/连发配 `Cooldown.Fire` GE，全自动留空 |
| `GA_Dash` | `GameplayAbility.Dash` | `State.Dashing` | `State.Dead`、`State.Dashing` | `Cooldown.Dash` |
| `GA_Heal` | `GameplayAbility.Heal` | `State.Healing` | `State.Dead`、`State.Healing` | `Cooldown.Heal` |
| `GA_SpawnBlock` | `GameplayAbility.SpawnBlock` | （空，纯瞬发） | `State.Dead` | `Cooldown.SpawnBlock` |

各条的理由：

1. **`GA_Fire` 三个模式共用一套标签。** 全自动/半自动/三连发的差异全在武器数据里（有没有冷却 GE、`FireCount` 是几），不该在标签层面产生分叉，否则每加一把枪就要动标签。
2. **`State.Fire` 是"别人查我"用的，不是"我判断自己"。** 全自动的"停止"不是标签，是 `InputReleased → EndAbility`；`ActivationOwnedTags` 会随 `EndAbility` 自动移除，**不需要 `GameplayAbility.Fire.Stop` 这类标签**。
3. **`GA_Dash` 虽然是瞬时技能，仍然需要状态标签。** 位移过程有持续时间，`State.Dashing` 从激活挂到 `EndAbility`。**瞬时 ≠ 无状态**——"瞬发"指不等待输入/不持续消耗，不代表效果瞬间结束。
4. **`GA_SpawnBlock` 是纯瞬发**，生成 Actor 后立即 `EndAbility`，没有需要维持的状态，`ActivationOwnedTags` 留空。障碍物默认雷元素是**生成物的数据**（`Element.Lightning`，该标签尚未在 ini 里声明），不是能力的标签。
5. **`GA_Heal` 两个标签都要**：`GameplayAbility.Heal` 让"被攻击中断"能匹配到它（`CancelAbilities` 只认 `AbilityTags`），`State.Healing` 让"治疗中禁止开火/冲刺"这类阻断生效。
