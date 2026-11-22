# FPS-v0.2.1

> **备份版本**：无新功能。本版本只为存档当前工作区状态，并记录"第三人称步枪网格体歪斜"问题的排查结论。

## 本版本范围

- 存档武器双网格（第一人称 / 第三人称）挂载链路的当前实现；
- 记录第三人称步枪歪斜问题的完整排查过程与根因定位；
- **修复尚未应用**：socket 迁移方案待原素材（原始角色资源包）到位后实施。

## 源码

- 新增 `IPlayerInterface`（`GetFirstPersonArms` / `GetThirdPersonMesh`）；
- `AGASCharacter`：新增 `FirstPersonCamera`（挂 SpringArm）与 `FirstPersonArms`（bOnlyOwnerSee），实现上述接口；
- `AWeapon`：新增 `FirstPersonMesh` / `ThirdPersonMesh` 双网格组件，`AttachToOwingPawn` 经接口分别挂到角色第一人称手臂与第三人称身体，`SetMeshVisibility` 按是否本地控制切换显隐；
- `UCombat`：新增武器库存与装备链路（`SpawnInventoryWeapons` / `EquipWeapon` / `OnRep_CurrentWeapon`）。

## 资产

- `BP_GASCharacter`、`ABP_FirstPerson` / `ABP_ThirdPerson`、`BP_Pistol`、`DA_Pistol`、`DA_Rifle`、`SK_Mannequin` 等同步调整；
- 武器挂点由数据资产指定：`DA_Rifle.GripPoints = RifleGripPoint`，`DA_Pistol.GripPoints = PistolGripPoint`。

## 本版本边界

- **已知问题（根因已定位，未修）**：第三人称步枪在动画中歪斜。`RifleGripPoint` / `PistolGripPoint` socket 挂在右手中指骨 `middle_01_r` 上，参考姿势（网格体预览）下正常，动画播放时指骨随手部姿势旋转，把枪带歪；手枪因动画姿势巧合不明显。修复方向：socket 迁至掌骨 `hand_r`。
- 已知隐患（未修）：`Weapon.cpp` `AttachToOwingPawn` 中 `WeaponData` 判空位于取值之后，存在空指针解引用风险。
- 开火链路仍不可用（同 v0.2.0）。
