# FPS-v0.2.4

> 动画层基本成型：Aim Offset、原地转身（TurnInPlace）、右手 FABRIK 持枪 IK、瞄准 FOV 缩放；新增 `Enums` 文件夹与 `ETurnInPlaceEnum`。原地转身存在已知 bug，非项目重点，不打算修复。

## 本版本范围

- 第三人称动画基本完成：站立 / 下蹲状态机 + Aim Offset + 原地转身状态 + 右手 FABRIK 持枪 IK；
- 瞄准视野（FOV）缩放：`CF_Aim_FOV` 曲线 + `OnAim` 蓝图事件，由 `GA_Aim_Base` 激活 / 结束驱动；
- 新增 `Source/FPS/Public/Enums` 与 `Source/FPS/Private/Enums` 文件夹，首个成员 `ETurnInPlaceEnum`（Left / Right / NoTurning），供转身逻辑在 C++ 与动画蓝图间共享；
- 修复 v0.2.3 遗留的 `OnRep_bIsAiming` 笔误。

## 源码

- `Enums/TurnInPlaceEnum.h/.cpp`（新增）：`ETurnInPlaceEnum` 蓝图枚举（Left / Right / NoTurning）；
- `GASCharacter.h/.cpp`：新增 `BeginPlay`（设置默认 FOV、记录 `StartingAimRotation`）；`Tick` 每帧驱动 `CalculateTurnInPlaceParameters` 与 `CalculateFABRIKTransform`；新增 `AO_Yaw` / `MovementOffsetYaw` / `TurningStatus` / `InterpSpeed`（BlueprintReadOnly，供 ABP Property Access 线程安全读取）；`TurnInPlace`：站桩时 `AO_Yaw` 超 ±90° 触发左 / 右转身并插值回零；`CalculateFABRIKTransform`：每帧把当前武器第三人称网格的 `FABRIK_Socket` 变换转换到 `hand_r` 骨骼空间，作为右手 IK 目标；`GetFixedRotation`：非本地角色俯仰角 [270, 360) 映射到 [-90, 0]；新增 `OnAim` BlueprintImplementableEvent（FOV 缩放在蓝图侧实现）；`FirstPersonCamera` 移为 public，新增 `DefaultFieldOfView = 105`；
- `GA_Aim_Base.cpp`：Activate / End 时回调 `OnAim(bIsAiming)`，驱动 FOV 表现；
- `Combat.cpp`：`OnRep_bIsAiming` 修复为向 Owner 转发 `OnAim`（旧实现把 `bIsAiming` 赋回旧值的笔误，v0.2.3 已记录待修）；
- `WeaponData.h`：新增 `AimFOV`（默认 90）与 `FABRIK_SocketName`；
- `GA_Dash_Base.h`：补 Activate / End 的注释骨架（暂不实现）；`Weapon.h` 清理空行；
- `Config/DefaultEngine.ini`：补两条重定向（`DefaulFieldOfView` 拼写修正、`GetFixedRotationClamped → GetFixedRotation`）。

## 资产

- `ABP_ThirdPerson`：重构为 Ground（Stand / Crouch）、Aim Offset、FABRIK 三个状态机并各自缓存姿势，经 `LayeredBoneBlend`（spine_01，深度 2）叠加，尾部接 `Orientation Warping` 节点（启用 AnimationWarping 插件）；Ground Standing / Crouching 内含 Idle / Run / TurnLeft / TurnRight 状态，转身由 `TurningStatus` 驱动，上半身 Aim Offset 由 `AO_Yaw` 驱动；
- `CF_Aim_FOV`（新增）：瞄准 FOV 插值曲线，配合 `OnAim` 在瞄准时收紧视野（`WeaponData.AimFOV` 为每武器目标值）；
- `BS_Strafing_Pistol` / `BS_Strafing_Rifle`：strafe 混合空间接入 `MovementOffsetYaw`；
- `DA_Pistol` / `DA_Rifle`：补 AimFOV 与 FABRIK socket 数据；`BP_GASCharacter`：实现 `OnAim` 的 FOV 过渡；
- `FPS.uproject`：启用 AnimationWarping 插件（朝向扭曲节点所需）与 AllToolsets（编辑器开发辅助）。

## 本版本边界

- **已知 bug，不修复**：原地转身一转向就全身整体跟随，做不到"上半身先转、下半身超过阈值才跟随"。已定位原因：`BP_GASCharacter` 的 `bUseControllerRotationYaw = true` 使胶囊体每帧跟随相机 Yaw，且朝向扭曲节点接在全身姿势上（静止时 Alpha = 1、OrientationAngle = AO_Yaw），下半身被实时扭曲。原地转身不是本项目重点，不打算修复；
- 能力授予仍是 demo 临时方案（`PossessedBy` 里直接授 C++ 类），待 GA 蓝图子类到位后替换；
- **下一步**：添加蒙太奇（Montage）并完善 `GA_Fire_Base`；尝试为 `GA_Fire_Base` 添加 cooldown（冷却）与全自动模式。
