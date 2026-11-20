# C++ 类结构图（FPS-v0.2.0 时点）

> 记录 `FPS-v0.2.0` 打版时 `Source/FPS` 的类结构与关系。后续版本结构变化时更新或另存新文件。

```mermaid
classDiagram
    %% ===== 引擎基类 =====
    class ACharacter
    class APlayerController
    class AGameModeBase
    class AActor
    class UActorComponent
    class UDataAsset
    class UAttributeSet
    class UGameplayAbility
    class IAbilitySystemInterface

    %% ===== 项目类 =====
    class AGASCharacter {
        +UAbilitySystemComponent* AbilitySystemComponent
        +UCombat* CombatComponent
        +UCameraComponent* FirstPersonCamera
        +USkeletalMeshComponent* FirstPersonArms
        +PossessedBy()
        +OnRep_PlayerState()
        -Input_Fire_Pressed()
        -Input_Fire_Released()
        -Input_Aim()
        -Input_CycleWeapon()
    }

    class AGASPlayerController {
        -UInputMappingContext* GASIMC
        -Input_Move()
        -Input_Look()
        -Input_Jump()
        -Input_Crouch()
    }

    class AGASGamemodeBase {
    }

    class UCombat {
        +Initiate_Fire_Pressed()
        +Initiate_Fire_Released()
        +Initiate_Aim()
        +Initiate_CycleWeapon()
    }

    class AWeapon {
        +UWeaponData* WeaponData
        +GetFirstPersonMesh()
        +GetThirdPersonMesh()
        -FGameplayTag WeaponType
    }

    class UWeaponData {
        +FName GripPoints
        +FPlayerAnims FirstPersonAnims
        +FPlayerAnims ThirdPersonAnims
        +TArray~TSubclassOf<UGameplayAbility>~ AbilitiesToGrant
    }

    class UFPSAttributeSet {
        +FGameplayAttributeData Health
        +FGameplayAttributeData MaxHealth
        +FGameplayAttributeData MoveSpeed
        +FGameplayAttributeData MaxMoveSpeed
        +GetLifetimeReplicatedProps()
    }

    class UGA_Base {
        +bool bShouldShowOnHUD
    }

    class UGA_Fire_Base {
    }

    %% ===== 继承 =====
    ACharacter <|-- AGASCharacter
    IAbilitySystemInterface <|.. AGASCharacter
    APlayerController <|-- AGASPlayerController
    AGameModeBase <|-- AGASGamemodeBase
    AActor <|-- AWeapon
    UActorComponent <|-- UCombat
    UDataAsset <|-- UWeaponData
    UAttributeSet <|-- UFPSAttributeSet
    UGameplayAbility <|-- UGA_Base
    UGA_Base <|-- UGA_Fire_Base

    %% ===== 组合 / 引用 =====
    AGASCharacter *-- UCombat : 持有（战斗输入入口）
    AGASCharacter --> AWeapon : CurrentWeapon（v0.2.0 已移除，待重建）
    AWeapon *-- UWeaponData : 数据资产
    UWeaponData --> UGameplayAbility : AbilitiesToGrant
    AGASPlayerController ..> AGASCharacter : 移动/视角输入
    UCombat ..> UGA_Fire_Base : 触发（规划中）
```

## 说明

- **输入分层**：`AGASPlayerController` 只负责移动 / 视角 / 跳跃 / 蹲伏；战斗输入（开火 / 瞄准 / 切枪）绑定在 `AGASCharacter::SetupPlayerInputComponent`，转发给 `UCombat` 组件的 `Initiate_*` 入口（当前为调试桩，v0.2.0 时点未接 GA）。
- **能力体系**：`UGA_Base`（原 `UGASGameplayAbilityBase` 迁移重命名）为全部能力基类；`UGA_Fire_Base` 是开火能力基类，空壳待实现。蓝图能力（`GA_Aim` / `GA_Heal` / `GA_SpawnBlock`）继承关系未画入本图。
- **属性体系**：`UFPSAttributeSet` 只保留角色固有属性（血量 / 移速）；弹药按方案 A 归属武器，本版本尚未在 `AWeapon` / `UWeaponData` 上落地字段。
