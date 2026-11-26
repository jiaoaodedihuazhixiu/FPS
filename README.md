# FPS — UE5 GAS 第一人称射击项目

基于 **Unreal Engine 5.8** + **GameplayAbilitySystem** 的第一人称射击项目，作为学习与作品集项目维护。

## 当前版本：FPS-v0.2.3（2026.11.26）

本版本说明（详细记录见 [Docs/Changelog/FPS-v0.2.3.md](Docs/Changelog/FPS-v0.2.3.md)）：

1. **瞄准能力落地**：`GA_Aim_Base` 完成 `ActivateAbility` / `EndAbility` 全链路 —— 按住右键激活、松开结束（`WaitInputRelease`），本地预测执行，激活持有 `State.Aiming` 标签；
2. **`Combat` 新增 `bIsAiming`**：复制变量（`ReplicatedUsing`），作为 ABP `Blend Poses by Bool` 的混合开关；瞄准输入拆分为 Pressed / Released 双通道，与开火一致走 ASC 通用输入事件；
3. **第一人称瞄准动画**：`ABP_FirstPerson` 新增 ADS 姿态，按 `bIsAiming` 在常态与瞄准姿态间混合；
4. **下一步目标**：补充瞄准视野（FOV 缩放）与瞄准偏移（Aim Offset）。

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
