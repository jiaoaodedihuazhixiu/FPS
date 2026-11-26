# FPS-v0.2.3

> 瞄准能力落地：`GA_Aim_Base` 完成 Activate / End 全链路；`Combat` 新增复制变量 `bIsAiming`；第一人称瞄准动画接入动画蓝图。

## 本版本范围

- `GA_Aim_Base` 从骨架填为可用能力：按住右键激活、松开结束，本地预测执行；
- `Combat` 组件新增 `bIsAiming`（Replicated），作为动画层 `Blend Poses by Bool` 的混合开关；
- `ABP_FirstPerson` 新增第一人称瞄准姿态，按 `bIsAiming` 在常态与瞄准姿态间混合；
- 瞄准输入拆分为 Pressed / Released 双通道，对接 ASC 的通用输入事件。

## 源码

- `GA_Aim_Base.h/.cpp`：完整实现。构造期设定 `InstancedPerActor` + `LocalPredicted`，资产标签 `Ability.Weapon.Aim`，激活持有 `State.Aiming`，被 `State.Dashing` / `State.Dead` 阻止；`ActivateAbility` 解析当前武器与 WeaponData（任一为空则直接结束），置 `bIsAiming = true`，挂 `UAbilityTask_WaitInputRelease`（`bTestAlreadyReleased=true`）等待松手；`OnInputReleased` 回调里 `EndAbility`；`EndAbility` 置 `bIsAiming = false`；新增 `ResolveWeapon` / `ResolveWeaponData` 辅助函数（经 Combat 组件取当前武器，带判空）；
- `Combat.h/.cpp`：`Initiate_Aim` 拆分为 `Initiate_Aim_Pressed` / `Initiate_Aim_Released`，分别调用 `ASC->AbilityLocalInputPressed/Released(Aim)` —— 瞄准的"按住"语义与开火一致，走 ASC 通用输入复制事件；新增 `bIsAiming`（`BlueprintReadOnly`，`ReplicatedUsing=OnRep_bIsAiming`）、`SetIsAiming()` 与 `OnRep_bIsAiming()`，复制注册进 `GetLifetimeReplicatedProps`；顺带清理 `Initiate_Fire_*` 里注释掉的 debug 代码；
- `GASCharacter.cpp/h`：`IA_Aim` 绑定改为 `Started → Input_Aim_Pressed` / `Completed → Input_Aim_Released`（含临时屏幕 debug 输出）；`PossessedBy` 在服务器端授予 `GA_Aim_Base`（InputID = Aim），原有 `GA_Fire_Base` 授予同步包进 `HasAuthority()` 判断；
- `GA_Fire_Base.cpp`：调整 `ActivateAbility` 内顺序 —— 先挂 `WaitInputRelease` 监听再 `StartFire`，避免弹药耗尽直接 `EndAbility` 时留下"能力已结束、任务还在等"的孤儿任务；`ResolveWeapon` / `ResolveWeaponData` 移至文件尾（逻辑不变）。

## 资产

- `ABP_FirstPerson`：新增第一人称瞄准姿态（ADS），AnimGraph 中用 `Blend Poses by Bool` 读取 `Combat.bIsAiming`（Property Access，线程安全），在常态与瞄准姿态间混合。

## 本版本边界

- 瞄准只到"姿态混合"为止：**瞄准视野（FOV 缩放）与瞄准偏移（Aim Offset）尚未做**，为下一版本目标；
- `OnRep_bIsAiming` 当前是占位实现（未接表现层逻辑），且实现里把 `bIsAiming` 赋回旧值，疑似笔误，待修；
- `Input_Aim_Pressed` / `Input_Aim_Released` 里的屏幕 debug 输出为临时调试用，后续移除；
- 能力授予仍是 demo 临时方案（`PossessedBy` 里直接授 C++ 类），待 GA 蓝图子类到位后替换。
