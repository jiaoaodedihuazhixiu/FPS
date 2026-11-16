# FPS-v0.1.0（2026.11.15）

> 本文件于 `FPS-v0.1.1` 从原 `CHANGELOG.md` 原样拆出，内容未改动。

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
