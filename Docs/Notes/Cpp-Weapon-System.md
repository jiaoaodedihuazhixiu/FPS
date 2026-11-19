# C++ 主导的武器系统（`cpp-dominant` 分支）

> 本分支把武器系统的「复制 / 挂载 / 可见性」三件事从蓝图搬到 C++，作为技术参考保留。
> `main` 分支继续用蓝图实现同一套玩法逻辑。

## 为什么值得单独留一条分支

蓝图能跑通，但跨端行为要靠人肉连节点保证：装备时挂哪层、哪一端去挂、谁看不见谁，全靠作者记得住。
C++ 版把规则**固化成引擎机制**：

- 可见性交给 `bOnlyOwnerSee` / `bOwnerNoSee`（按视角求值，天然分端正确）；
- 挂载关系拆成「Actor 级（随复制下发）」和「组件级（各端自建）」两条路径；
- 三个入口（BeginPlay / SetOwner / OnRep_Owner）互斥且完备，不再依赖 BP 里的调用顺序。

## 一页架构

```text
AWeapon (bReplicates = true, bNetUseOwnerRelevancy = true)
├─ WeaponRoot        (USceneComponent)   ← root，只负责"引擎认为它在哪"
├─ FirstPersonMesh   (SkeletalMesh)      ← SetOnlyOwnerSee(true)，只有本人可见
└─ ThirdPersonMesh   (SkeletalMesh)      ← SetOwnerNoSee(true)，本人不可见、别人可见
```

装备流程（服务器）：

```text
SpawnActorDeferred(BP_Pistol)
  → SetOwner(Character)                  // 决定可见性 + 网络相关性 + RPC 归属
  → FinishSpawning
       ├─ BeginPlay()                    // 两端都会跑
       │    → AttachToOwner()
       │         ├─ ① Actor root  → Character.Mesh 的 GripPoint socket（仅 authority，走 FRepAttachment 复制）
       │         └─ ② FP mesh    → Character.FirstPersonArms 的 GripPoint socket（两端各挂一次）
       └─ OnRep_Owner()                  // 客户端收到 owner 后补挂
```

## 三个入口为什么互斥且完备

| 时机 | 负责挂载的函数 | 说明 |
| --- | --- | --- |
| spawn 时就设好 owner（延迟生成） | `BeginPlay()` | `SetOwner` 发生时组件还没注册，交给 BeginPlay |
| spawn 之后运行中再设 owner（服务器） | `SetOwner()` override | `HasActorBegunPlay()` 为真才执行 |
| 客户端收到 owner 复制 | `OnRep_Owner()` | `SetOwner` 在服务器不会触发 RepNotify |

## 关键引擎事实（实现时踩过的点）

1. **`Owner` 决定可见性**：渲染代理会收集组件 owner 链上所有 Actor 的 UniqueID，与视角的 `View.ViewActor`
   比对。没有 Owner，`bOnlyOwnerSee` 的枪谁都看不见，`bOwnerNoSee` 的枪谁都看得见 —— 正好反了。
2. **`Owner` 决定网络相关性**：`IsNetRelevantFor` 里 `IsOwnedBy(ViewTarget)` 恒真，
   `bNetUseOwnerRelevancy` 还会把相关性/优先级转发给 Owner。没 Owner 时枪会因为距离被剔除。
3. **Actor 挂载会复制，组件挂载不会**：`FRepAttachment`（parent + 组件 + socket + 相对变换）随 Actor 复制；
   而 `USceneComponent::AttachParent` 只有组件自身 `bReplicates = true` 才复制，`SkeletalMeshComponent` 默认不复制。
   所以 FP mesh 必须两端各挂一次 —— 这是正确性要求，不是优化。
4. **不要在组件注册前挂载**：`SpawnActorDeferred` 阶段 `AttachToComponent` 只是登记待挂载状态，
   而 `SnapToTargetNotIncludingScale` 的缩放规则是 `KeepWorld`，注册前算相对缩放会用到未初始化的
   `ComponentToWorld`。
5. **`bHiddenInGame` 不参与复制**：复制的只有 `USceneComponent::bVisible`。用它做第一/第三人称区分，
   在服务器上改不会同步到客户端；而且 `bHiddenInGame` 会直接盖掉 `bOnlyOwnerSee`。

## 文件地图

| 文件 | 职责 |
| --- | --- |
| `Source/FPS/Public/Weapon/Weapon.h` | `AWeapon` 声明：root + 两把 mesh、挂载入口、socket 查询 |
| `Source/FPS/Private/Weapon/Weapon.cpp` | 构造期可见性设置、`AttachToOwner` / `ResetAttachment`、三个入口 |
| `Source/FPS/Public/Weapon/Data/WeaponData.h` | `UWeaponData`：握把 socket 名、第一/第三人称动画集、授予的能力 |
| `Source/FPS/Public/Character/GASCharacter.h` | `FirstPersonArms` 及其 getter（供武器挂载使用） |

## 与蓝图实现的对应关系

| C++ | 蓝图等价物 | 注意 |
| --- | --- | --- |
| `SetOwner(Character)` | `Set Owner` 节点 | 必须在挂载前设置 |
| `AttachToComponent(Character->GetMesh(), ...)` | `Attach Actor To Component` | 会复制，服务器做一次即可 |
| `FirstPersonMesh->AttachToComponent(Arms, ...)` | `Attach Component To Component` | **不复制**，两端都要做 |
| `SetOnlyOwnerSee(true)` / `SetOwnerNoSee(true)` | 组件 Details 里勾选 | 类默认值即可，不要运行时切 |
| `GetGripSocketName()` | `WeaponData.GripPoints` 读取 | 数据资产里填 `PistolGripPoint` / `RifleGripPoint` |

## 可写进简历的表述（参考）

> 在 UE5 + GAS 的第一人称射击项目中，用 C++ 重构武器系统的复制与挂载层：以 `Owner` 驱动
> `bOnlyOwnerSee` / `bOwnerNoSee` 实现第一/第三人称武器分视角可见；区分 Actor 级（随 `FRepAttachment` 复制）
> 与组件级（各端自建）挂载路径，解决 listen server 与远端客户端行为不一致的问题；并结合动画蓝图的
> Property Access 在生产线程侧消费武器数据，避免线程安全违规。
