# FPS-v0.2.2

> 开火链路第一步落地：射线检测打通；GameplayTag 从 ini 迁移为 C++ native tag；搭好瞄准 / 冲刺能力骨架。

## 本版本范围

- `GA_Fire_Base` 与 `AWeapon` 完成射线检测（hitscan）：开火时从玩家视点做球形扫描，拿到命中结果；
- 新增 native GameplayTag 模块，把 C++ 要引用的标签从 `DefaultGameplayTags.ini` 迁出；
- 新增 `GA_Aim_Base` / `GA_Dash_Base` 骨架类（仅类壳，逻辑待填）。

## 源码

- 新增 `Tags/FPSTags.h` / `FPSTags.cpp`：项目内所有 C++ 侧 GameplayTag 的集中声明与定义（`UE_DECLARE/DEFINE_GAMEPLAY_TAG`），含 Ability.* / State.* / Event.* / Data.* / Cooldown.* 分层约定；native tag 在模块静态构造期注册，可安全用于构造函数，且拼写错误是编译错误；
- 新增 `GA_Aim_Base`（Public/Private 各一）：瞄准能力骨架，继承 `GA_Base`；
- 新增 `GA_Dash_Base`（Public/Private 各一）：冲刺能力骨架，继承 `GA_Base`；
- `FPS.h`：新增 `FPSTraceChannels::ECC_Weapon = ECC_GameTraceChannel2`（避开已占用的 Projectile 通道）；
- `WeaponData.h`：新增 `FMontages` 结构（Equip / Fire / Reload）与第一人称、第三人称、武器三组 Montage 槽位；新增 `Range`（默认 5000）与 `TraceRadius`（默认 5）射线参数；
- `Weapon.cpp/h`：新增 `WeaponTrace()` —— 从 PlayerController 视点出发，按 `WeaponData->Range` 做 `SweepSingleByChannel`（ECC_Weapon、球形半径 `TraceRadius`），只 Block Pawn / WorldStatic / WorldDynamic / PhysicsBody，忽略武器 Owner，附 debug 球体轨迹绘制；
- `GA_Fire_Base.cpp/h`：`FireOnce()` 在服务器端（`HasAuthority`）调用 `WeaponTrace` 取命中结果；`StartFire` / `StopFire` 连通输入侧，`StopFire` 先判 `IsActive()` 防重复 `EndAbility` 导致标签计数漂移；
- `GA_Base.h`、`GASCharacter.cpp`、`Combat.cpp/h` 同步调整。

## 资产

- 删除蓝图能力资产 `GA_Heal` / `GA_SpawnBlock` / `GA_TestAbility`（能力实现收进 C++）；
- `DA_Pistol` / `DA_Rifle` 随 WeaponData 新字段同步调整；
- `DefaultGameplayTags.ini`：移除已迁移为 native tag 的条目，仅保留策划向的 `Weapon.Type.*`；
- `DefaultEngine.ini`：注册 `Weapon` 追踪通道（ECC_GameTraceChannel2，默认 Block）。

## 本版本边界

- 射线只到"拿到 FHitResult"为止：伤害结算、TargetData 上报尚未做（代码注释里标了"先做服务器自算，后面再改成 TargetData"）；
- `GA_Fire_Base::FireOnce` 中 ① 弹药检查 + CommitAbilityCost、② PlayMontageAndWait 两段仍是注释占位，未实现；
- `GA_Aim_Base` / `GA_Dash_Base` 只有空类壳；
- 下一版本目标：补完动画（Montage 资产 + 动画蓝图接线），完成 `GA_Fire_Base` 里的 PlayMontageAndWait。
