# 版本记录

> **项目**：FPS（`D:\Unreal Engine Project\FPS`）
> **引擎**：UE 5.8（引擎目录 `D:\Unreal Engine\UE_5.8`）
> **版本格式约定**：标签统一写作 `FPS-v<主>.<次>.<修订>`，例如 `FPS-v0.1.0`。标签指向**含该版本说明的提交**。
> **记录约定**：本文件只做索引，一行一版，最新在最上面。每个版本的详细说明在 `Docs/Changelog/FPS-v<版本>.md`，**写完不再修改**；发版时新建说明文件、在本表加一行、把标签打在该提交上。
> **仓库约定**：本项目保存在本地仓库并推送至 GitHub 远端（`origin`）。

---

## 版本索引

| 版本 | 日期 | 摘要 | 说明文件 |
| --- | --- | --- | --- |
| `FPS-v0.2.1` | 2026.11.22 | **备份版本，无新功能**：存档武器双网格挂载链路；记录第三人称步枪歪斜根因（socket 挂在指骨 middle_01_r），修复待原素材到位后实施 | [FPS-v0.2.1](Docs/Changelog/FPS-v0.2.1.md) |
| `FPS-v0.2.0` | 2026.11.20 | 放弃全蓝图 GA 方案，确立 C++ 主干 + 蓝图表现混合架构；新增 FPSAttributeSet / GA_Base / GA_Fire_Base / UCombat，删除蓝图开火族与 GA_Dash；开火链路暂不可用 | [FPS-v0.2.0](Docs/Changelog/FPS-v0.2.0.md) |
| `FPS-v0.1.1` | 2026.11.16 | GA_FireModeBase 开火流程；新增 Projectile 通道与 Bullet 预设，修掉子弹被自身射线命中导致的准星上跳 | [FPS-v0.1.1](Docs/Changelog/FPS-v0.1.1.md) |
| `FPS-v0.1.0` | 2026.11.15 | 项目初始化：角色 / 武器基础设置、武器数据类、角色控制器与能力基类 | [FPS-v0.1.0](Docs/Changelog/FPS-v0.1.0.md) |

## 发版流程

1. 代码与资产改完、自测通过；
2. 新建 `Docs/Changelog/FPS-vX.Y.Z.md`，按「本版本范围 / 源码 / 资产 / 本版本边界」四段写；
3. 在上面索引表**最上面**加一行；
4. 提交，然后 `git tag -a FPS-vX.Y.Z -m "<一句话>"` 打在同一个提交上，推送分支与标签（`git push origin <分支>` + `git push origin FPS-vX.Y.Z`）。

## 其它文档

- [分清 `AbilityTags` 与 `ActivationOwnedTags`](Docs/Notes/GameplayTags.md)：标签体系的知识笔记，与版本无关，不随版本翻滚。
