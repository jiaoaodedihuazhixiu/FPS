# FPS — UE5 GAS 第一人称射击项目

基于 **Unreal Engine 5.8** + **GameplayAbilitySystem** 的第一人称射击项目，作为学习与作品集项目维护。

## 当前版本：FPS-v0.2.4（2026.11.28）

本版本说明（详细记录见 [Docs/Changelog/FPS-v0.2.4.md](Docs/Changelog/FPS-v0.2.4.md)）：

1. **动画基本完成**：第三人称 `ABP_ThirdPerson` 落地站立 / 下蹲状态机、Aim Offset（`AO_Yaw` 驱动上半身）、原地转身状态（`TurningStatus` 驱动）与右手 FABRIK 持枪 IK（启用 AnimationWarping 插件）；
2. **新增 Enums 文件夹**：`Source/FPS/Public/Enums` 与 `Source/FPS/Private/Enums`，首个成员 `ETurnInPlaceEnum`（Left / Right / NoTurning），供 C++ 与动画蓝图共享；
3. **瞄准 FOV 缩放**：新增 `CF_Aim_FOV` 曲线与 `OnAim` 蓝图事件，瞄准时按 `WeaponData.AimFOV` 收紧视野；顺带修复 `OnRep_bIsAiming` 笔误；
4. **已知问题**：原地转身存在 bug —— 一转向就全身整体跟随，做不到上半身先转、下半身延迟跟随；已定位原因（`bUseControllerRotationYaw` + 朝向扭曲作用于全身姿势），但这不是项目重点，不打算修复；
5. **下一步目标**：添加蒙太奇（Montage）并完善 `GA_Fire_Base`；尝试为 `GA_Fire_Base` 添加 cooldown（冷却）与全自动模式。

## 分支说明

| 分支 | 实现方式 | 定位 |
| --- | --- | --- |
| `main` | 蓝图主导：武器管理组件、动画蓝图、GAS 能力均在蓝图里搭建 | 日常开发主线 |
| **`cpp-dominant`** | **C++ 主导**：武器的挂载、可见性、Owner 驱动的复制时序全部在 C++ 层实现，蓝图只负责填数据 | 技术参考分支，不合并进 `main` |

`cpp-dominant` 分支存在的意义：把武器系统的"正确做法"用 C++ 固化下来 —— 分层清晰、服务器与客户端行为一致、
全部基于引擎自带的复制与可见性机制，而不是靠蓝图里手连节点去保证。详细说明见
[Docs/Notes/Cpp-Weapon-System.md](Docs/Notes/Cpp-Weapon-System.md)。

## 技术栈

- Unreal Engine 5.8（C++ 与蓝图混合开发）
- GameplayAbilitySystem：`AbilitySystemComponent` / `GameplayAbility` / `GameplayTag` / `GameplayEffect`
- 增强输入（Enhanced Input）
- 动画蓝图 + Property Access（线程安全更新，AnimGraph 在 worker thread 求值）
- Git LFS 管理 `uasset` / `umap` 等二进制资源

## 目录结构

```
Source/FPS/            C++ 源码（Public / Private）
Content/GAS/           角色、武器、能力、输入、动画等资产
Docs/Changelog/        按版本拆分的变更记录
Docs/Notes/            知识点与实现笔记
Config/                项目配置（GameplayTag、输入、渲染等）
```
