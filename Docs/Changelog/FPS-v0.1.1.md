# FPS-v0.1.1（2026.11.16）

> **本版本范围**：把开火链路从零打通到“能打出去、能命中”——`GA_FireModeBase` 蓝图及其三个开火模式子类、子弹 Actor、武器管理组件；并用碰撞通道/预设把子弹从射线里摘出去，修掉连续开火时准星越打越高的正反馈。角色动画、武器数据表、伤害结算、弹药与换弹仍未进入本版本。

## 源码（`Source/FPS`）

| 类 | 文件 | 变更 |
| --- | --- | --- |
| `AGASCharacter` | `Character/GASCharacter.h` / `.cpp` | 实现 `IAbilitySystemInterface`；构造函数里创建 `AbilitySystemComponent`（`SetIsReplicated(true)` + `SetReplicationMode(ASCReplicationMode)`）；新增公开成员 `AbilitySystemComponent`（`EditAnywhere` + `BlueprintReadOnly`）与 protected 的 `ASCReplicationMode`（默认 `Mixed`）；新增 `GetAbilitySystemComponent()`、`PossessedBy()`、`OnRep_PlayerState()`，后两者都调用 `InitAbilityActorInfo(this, this)`；移除了原来的 `BeginPlay` 重写 |

## 资产（`Content`）

| 路径 | 类型 | 说明 |
| --- | --- | --- |
| `GAS/Abilities/Fire/GA_FireModeBase` | 蓝图（`UGA_FireBase`） | 开火基类。标签：`AbilityTags = GameplayAbility.Fire`、`ActivationOwnedTags = State.Fire`、`ActivationBlockedTags = State.Dead / State.Realoading / State.Dashing`；`InstancedPerExecution` + `LocalPredicted`。流程：`ActivateAbility → CommitAbility → StartFire → StopFire`；`FireOnce` 里 `WaitTargetData`（`GameplayAbilityTargetActor_SingleLineTrace`，`Instant`，`bTraceAffectsAimPitch = true`，`MaxRange = 999999`）→ `ValidData` → 生成 `BP_Bullet`（`SpawnLocation` = 角色位置，`TargetLocation` = `Get Target Data End Point`，`Speed = 2000`） |
| `GAS/Abilities/Fire/GA_SemiAuto`、`GA_FullAuto`、`GA_Burst` | 蓝图（父类 = `GA_FireModeBase`） | 半自动 / 全自动 / 三连发三个开火模式。目前只是空子类，没有自己的节点 |
| `GAS/Projectiles/BP_Projectile_Base` | 蓝图（`AActor`） | 子弹基类：`SphereCollision`（`bGenerateOverlapEvents`）+ `ProjectileMovement`；`BeginPlay` 用 `GetDirectionUnitVector(自身位置, TargetLocation) * Speed` 设 `Velocity` 再 `Activate`；`BeginOverlap` / `Hit` 都直接 `DestroyActor` |
| `GAS/Projectiles/BP_Bullet` | 蓝图（父类 = `BP_Projectile_Base`） | `GA_FireModeBase` 实际生成的子弹类 |
| `GAS/Components/WeaponManagerConponent` | 蓝图（`ActorComponent`） | 武器管理组件（新增），已挂到 `BP_GASCharacter`；目前只在 `BeginPlay` 里 `GetOwner → Cast` 缓存 `CurrentCharacter` |
| `GAS/Character/BP_GASCharacter` | 蓝图 | 挂载 `WeaponManagerConponent`；`Possessed` 里调 `GiveAbilities(OnlyTest)` 授予 `GA_Dash` + `GA_FireModeBase`；左键 `TryActivateAbilityByClass(GA_FireModeBase)`、左 Shift 冲刺；本次还留了几处调试用 `Print String` |
| `GAS/Abilities/GA_Dash` | 蓝图（`UGASGameplayAbilityBase`） | 冲刺能力，本版本随开火链路一起调整 |
| `GAS/Abilities/GA_TestAbility` | 蓝图 | 测试用能力 |
| `Basic_VFX/` | 素材包（40 个文件） | 15 个 Niagara 模板（`NS_Basic_1..15`）、13 个材质、2 个网格、6 张贴图，以及 1 张演示地图（含 `BuiltData`）与 2 个演示用资源 |

**碰撞配置（`Config/DefaultEngine.ini`）**

| 项 | 值 |
| --- | --- |
| 新物体通道 | `Projectile`（`ECC_GameTraceChannel1`），默认响应 = **阻挡** |
| 新预设 `Bullet` | `QueryOnly`，ObjectType = `Projectile`；WorldStatic / WorldDynamic = 阻挡，Pawn / PhysicsBody / Vehicle / Destructible = 重叠，**Projectile = 忽略**，Visibility / Camera = 忽略 |
| 已有预设调整 | `Pawn`、`CharacterMesh` 对 `Projectile` = 重叠；`Spectator` 对 `Projectile` = 忽略 |
| 预设更名 | `Weapon` → `Bullet`（靠 `ProfileRedirects` 兼容旧引用） |

> **解决的问题**：连续开火时 `WaitTargetData` 的射线会打到上一发留在枪口前的子弹，命中点又被 `bTraceAffectsAimPitch` 折进下一发朝向，形成“准星越打越高”的正反馈。把子弹放到独立物体通道、让射线不再命中它之后，射线只命中世界与角色。

## 本版本边界（据资产与配置核对）

1. **`TraceProfile` 仍是 `NoCollision`**：该预设在 ini 里只显式忽略了 Visibility / Camera，其余物体通道走通道默认（阻挡），所以它实际等价于“除可见性/相机外全部阻挡”。语义容易被误读，后续应换成专用射线预设（`Bullet` 或新建 `Weapon`）。
2. **开火是“一次激活 = 打一发”**：`ActivateAbility` 里 `StartFire` 之后紧接着 `StopFire → EndAbility`，`State.Fire` 在同一帧挂上又摘掉；按住连发 / 松开停火尚未实现。
3. **`Instant` + `bDebug` 看不到调试线**：`ConfirmTargetingAndContinue` 会先把 `bDebug` 置 false，随后目标选择器 Actor 立即销毁；要调射线得自己 `Draw Debug Line`。
4. **命中不结算任何东西**：`ValidData` 之后只生成子弹，没有伤害、GameplayEffect、命中特效或 Niagara 绑定；子弹本身也没有可见网格（只有 Sphere + Arrow 组件）。
5. **`GA_Heal` / `GA_SpawnBlock`** 仍是 v0.1.0 的占位状态（本版本未改动）。
6. **`UWeaponData` 仍只有 `GripPoints`**：射速、伤害、弹药、冷却等字段都没有，武器数据表尚未开始；三个开火模式也还没有数据驱动。
7. **动画未接入**：本版本只引入了 `Basic_VFX` 素材，没有任何 AnimBluePrint / 动画资源；`WeaponManagerConponent` 也只有最基础的缓存逻辑，武器切换、持有武器、开火入口都没做。
8. **能力授予是临时方案**：`GiveAbilities(OnlyTest)` 挂在 `Possessed` 上且没有 `Switch Has Authority`，客户端会本地再给一份 spec；`bSpawnOnlyOnAuthority` 为 false，客户端与服务端可能各生成一颗子弹。
