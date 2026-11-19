# Unreal MCP 配置进度

> 状态：**✅ 全链路已验证通过**：双击自动启动 → initialize 握手 → tools/list → 真实工具调用 → Claude Code connected / authenticated / Tools: 3
> 更新日期：2026-11-17（依据 `Saved/Logs/FPS.log` 复核）
> 依据：Epic 官方文档 [Unreal MCP in Unreal Editor（UE 5.8）](https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?application_version=5.8)

---

## 1. 目标

在本工程（`FPS`）中按 Epic 官方文档启用 Unreal MCP，使 Claude Code 能通过本地 HTTP 驱动编辑器。

## 2. 环境事实（已核实）

| 项目 | 值 |
| --- | --- |
| 工程文件 | `D:\Unreal Engine Project\FPS\FPS.uproject` |
| EngineAssociation | `5.8` |
| 引擎安装 | `D:\Unreal Engine\UE_5.8`（5.8.4，CL 58902052，`++UE5+Release-5.8`） |
| MCP 服务器地址 | `http://127.0.0.1:8000/mcp`（仅回环，无鉴权） |
| 端口 8000 | 空闲（文档同步时编辑器未运行） |
| MCP 客户端 | Claude Code **2.1.326**（`claude`，已安装）；VS Code 已安装；无 `codex` / `gemini` / `cursor` CLI |

### 插件状态（`FPS.uproject`，均已启用）

| 插件 | 作用 |
| --- | --- |
| `ModelContextProtocol` | MCP 服务器本体（文档中的 "Unreal MCP"） |
| `AllToolsets` | 加载全部默认工具集（**5.8 必须**，否则无工具） |
| `Terminal` | 可选，编辑器内终端 |
| `ToolsetRegistry` | 依赖项，由引擎自动启用 |

插件二进制已存在：`Engine/Plugins/Experimental/ModelContextProtocol/Binaries/Win64/UnrealEditor-ModelContextProtocol*.dll`。

## 3. 已完成

### 3.1 `{root}/.mcp.json`（新建）

Claude Code 的服务器清单，与 `ModelContextProtocol.GenerateClientConfig ClaudeCode` 的输出格式一致：

```json
{
  "mcpServers": {
    "unreal-mcp": {
      "type": "http",
      "url": "http://127.0.0.1:8000/mcp"
    }
  }
}
```

- JSON 解析校验通过。
- 未被 `.gitignore` 忽略，可随仓库共享（符合文档期望：从工程根目录启动代理即可发现）。

### 3.2 `Config/DefaultEditorPerProjectUserSettings.ini`（新建）

```ini
[/Script/ModelContextProtocolEngine.ModelContextProtocolSettings]
ServerUrlPath=/mcp
ServerPortNumber=8000
bAutoStartServer=True
bEnableToolSearch=True
```

为什么不只靠编辑器偏好设置：

1. **UE 5.8 的 `bAutoStartServer` 默认是 `false`**（5.9 才改为 `true`），不写就不会随编辑器启动。
2. 该设置类标注为 `config=EditorPerProjectUserSettings`，个人改动落在 `Saved/Config/WindowsEditor/EditorPerProjectUserSettings.ini`，而 `Saved/` 被 `.gitignore` 忽略，**无法共享给团队**；工程 `Config/Default*.ini` 才是可提交的基线层。
3. 键名与段名取自引擎源码
   `Engine/Plugins/Experimental/ModelContextProtocol/Source/ModelContextProtocolEngine/Public/ModelContextProtocolSettings.h`，
   且与本机已有 `Saved` 配置里的段完全一致（见下）。

### 3.3 现状核对（只读）

本机 `Saved/Config/WindowsEditor/EditorPerProjectUserSettings.ini` 里本就存在：

```ini
[/Script/ModelContextProtocolEngine.ModelContextProtocolSettings]
ServerUrlPath=/mcp
ServerPortNumber=8000
bAutoStartServer=True
bEnableToolSearch=True
```

即当前这台机器上自动启动本来就是开的；新增工程 ini 的作用是让基线可共享、并在新机器上默认生效。

## 4. 验证结果

**静态验证（已完成）**

- `.mcp.json` 结构合法，与官方示例逐字段一致（`type: http` + `url`）。
- ini 段名与键名对得上引擎源码中的 `UModelContextProtocolSettings`。
- 插件已启用且二进制存在。

**服务端端到端验证（2026-11-17 10:31:31–10:36:33，已通过）**

依据 `Saved/Logs/FPS.log` 复核（该次会话正常退出：`LogExit: Exiting`，无崩溃）：

| 环节 | 日志证据 |
| --- | --- |
| 服务器启动 | `LogModelContextProtocol: Starting MCP server on port 8000`（日志 10:31:31 打开，10:32:15 起服务，约 44 秒） |
| 协议握手 | `negotiated '2025-06-18'` → `Initializing new session: 1a73fd5c…` → `Session initialized` |
| 工具发现 | `list_toolsets` / `describe_toolset` 多次调用；工具集上限 **52** 个 |
| 真实工具调用 | `call_tool` → `ToolsetRegistry.AgentSkillToolset.ListSkills`、`EditorToolset.EditorAppToolset.GetSelectedActors`、`GetContentBrowserPath` |
| 多客户端 | 10:34:43 又建立一次会话（协议 `2025-11-25`），同一服务端可被不同客户端复用 |

只读调用，未执行资产写入、PIE 或代码修改。

上述那次是**带命令行参数**启动的（`-ModelContextProtocolStartServer` + `-ini:` 覆盖 + `-ModelContextProtocolPort=8000`），因此当时只能证明"服务端能力"，不能证明"双击自动起服务"。

**「双击启动 → 自动起服务」验证（2026-11-17 15:06，已通过）**

- 日志 `LogInit: Command Line:` 为**空**，即双击启动、未带任何 MCP 参数。
- frame `[0]` 就出现 `LogModelContextProtocol: Starting MCP server on port 8000`，随后 `LogHttpServerModule: Starting all listeners...` / `All listeners started`。
- 结论：`bAutoStartServer=True` 在普通启动下**确实生效**，原待办项关闭。
- 归因说明：工程 `Config/DefaultEditorPerProjectUserSettings.ini` 与本机 `Saved/…/EditorPerProjectUserSettings.ini` 两层值相同，故无法区分是哪一层生效；工程 ini 的价值在于让新机器/新成员默认开启。
- **不需要**再手动敲 `ModelContextProtocol.StartServer`：重复执行会先停掉现有服务器再重启监听（日志 `Warning: MCP server is already running on port 8000, stopping first.`），会踢掉已连接的客户端。

**实时握手复验（2026-11-17 15:11，已通过）**

直接对 `http://127.0.0.1:8000/mcp` 发 JSON-RPC：

| 请求 | 结果 |
| --- | --- |
| `initialize` | HTTP 200，`protocolVersion: 2025-06-18`，响应头带 `Mcp-Session-Id` |
| `notifications/initialized` | 200 |
| `tools/list` | 200，返回 3 个元工具：`list_toolsets` / `describe_toolset` / `call_tool`（工具搜索模式正常） |
| `tools/call list_toolsets` | 200，返回真实工具集清单（`ToolsetRegistry.AgentSkillToolset`、`ConfigSettingsToolset`、`AutomationTestToolset`、`DataRegistryToolset`…） |
| 编辑器日志侧 | `Client requested protocol version '2025-06-18'` → `Initializing new session: dc40e5ac…`，证明请求确实进了编辑器 |

自测脚本（任意终端可复用）：

```powershell
$base='http://127.0.0.1:8000/mcp'
$init='{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"protocolVersion":"2025-06-18","capabilities":{},"clientInfo":{"name":"probe","version":"1"}}}'
$r=Invoke-WebRequest $base -Method Post -Body $init -ContentType 'application/json' -Headers @{'Accept'='application/json, text/event-stream'} -UseBasicParsing
$sid=$r.Headers['Mcp-Session-Id']; $h=@{'Accept'='application/json, text/event-stream';'Mcp-Session-Id'=$sid}
Invoke-WebRequest $base -Method Post -Body '{"jsonrpc":"2.0","method":"notifications/initialized"}' -ContentType 'application/json' -Headers $h -UseBasicParsing | Out-Null
(Invoke-WebRequest $base -Method Post -Body '{"jsonrpc":"2.0","id":2,"method":"tools/list"}' -ContentType 'application/json' -Headers $h -UseBasicParsing).Content
```

**一个容易误解的点**：`Log LogModelContextProtocol Verbose` 执行后**没有任何输出是正常的**——`Log` 是静默设置命令，只在 Output Log 留一行 `Cmd:` 回显，不回显当前级别。5.8 里 MCP 的关键信息本就以 `Log` 级别输出，不先开 Verbose 也能看到。

**Claude Code 客户端验证（2026-11-17 15:2x，已通过）**

用户在工程根目录启动 `claude`，首次弹出项目级 MCP 信任提示（`New MCP server found in this project: unreal-mcp`），选择 `1. Use this MCP server` 后，`/mcp` 详情页显示：

```
Status:         connected
Auth:           authenticated
URL:            http://127.0.0.1:8000/mcp
Config location: D:\Unreal Engine Project\FPS\.mcp.json
Capabilities:   tools
Tools:          3 tools
```

- **`Tools: 3` 是正常的，不是缺工具**：`bEnableToolSearch=True`（默认）时，客户端只拿到 3 个元工具 `list_toolsets` / `describe_toolset` / `call_tool`，几十个工具集按需通过 `call_tool` 调用，避免 `tools/list` 返回巨大 schema。想让客户端直接看到所有工具，可把 ini 里 `bEnableToolSearch` 改为 `False`（代价是首次响应很大，不推荐）。
- 信任提示选了 `1`（仅本次），没选 `2. Use this and all future MCP servers in this project`——因为 `.mcp.json` 随仓库分发，选 2 等于以后仓库里出现的任何 MCP 服务器都自动执行、不再询问，有供应链风险。
- 环境侧历史（已解决）：CLI 安装在 `%APPDATA%\npm\claude.cmd` / `claude.ps1`，`claude.cmd --version` → `2.1.326`；PowerShell 会把 `claude` 解析到 `claude.ps1` 并被 `Restricted` 执行策略拦截（`Get-ExecutionPolicy -List` 全为 `Undefined`），用户已将当前用户策略改为 `RemoteSigned` 解决。`claude mcp list` 在无 TTY 环境会挂起，仅影响非交互式自检。

## 5. 日常使用与剩余事项

1. 服务端配置无需再改：双击 `FPS.uproject` 即自动起服务（2026-11-17 15:06 已验证），**不需要**手动 `ModelContextProtocol.StartServer`。
2. 若 Output Log 中没有 `LogModelContextProtocol` 的监听记录，用控制台命令 `ModelContextProtocol.StartServer` 手动启动，并检查 `Log LogModelContextProtocol Verbose`。
3. Claude Code 启动方式（已验证通过，新机器照此操作）：
   1. 解除脚本策略拦截（一次性，普通用户即可）：
      `Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy RemoteSigned`，然后**新开**窗口用 `claude --version` 确认（`2.1.326`）。不想改策略就一律用 `claude.cmd`。
   2. 先启动编辑器，确认 Output Log 有 `Starting MCP server on port 8000`。
   3. **在工程根目录**（`.mcp.json` 所在目录）执行 `cd "D:\Unreal Engine Project\FPS"` 后再 `claude`——目录不对就发现不了服务器。
   4. 首次启动会询问是否信任项目级 MCP 服务器（`.mcp.json` 里的 `unreal-mcp`），选 `1. Use this MCP server`。
   5. `/mcp` 应显示 `connected` / `authenticated` / `Tools: 3`（3 个元工具，正常）；若 failed，先查端口 `netstat -ano | findstr :8000`。
4. 可使用的冒烟测试提示词：`我选中了哪些 Actor？` 或 `有哪些工具集可用？`（本次已通过等价只读调用）。
5. 若 8000 端口被占用：改本文件第 3.2 节 ini 中的 `ServerPortNumber`，并同步改 `.mcp.json` 的 `url`（或直接在编辑器里点 **Generate Client Config** 让它重写 URL）。
6. 需要 VS Code Copilot / Codex 等其它客户端时，在编辑器控制台执行
   `ModelContextProtocol.GenerateClientConfig All`（对应 `Cursor`/`VSCode`/`Gemini`/`Codex`），本工程目前**只**配置了 Claude Code。

## 6. 5.8 与 5.9 文档差异（避免照 5.9 文档配置出错）

| 项 | 5.8（本工程） | 5.9 |
| --- | --- | --- |
| `bAutoStartServer` 默认值 | `false`（所以需显式写 ini） | `true` |
| `serverInfo.name` | 返回空字符串（`name`/`title`/`version` 皆空） | 固定为 `unreal-mcp` |
| 服务器指令（Instructions）项目设置 | **不存在**，无法按 5.9 文档在 `Project Settings > Agentic Workflows` 配 | 支持，写入 `DefaultGame.ini` |
| 其它允许的源（Allowed Origins） | 无此设置 | 支持 |
| 媒体落盘 / 分页等 CVar | 无 | 有 |

## 7. 参考

- [Unreal MCP in Unreal Editor（5.8）](https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?application_version=5.8)
- [中文版文档](https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?lang=zh-CN)
- [ModelContextProtocol 插件 API](https://dev.epicgames.com/documentation/unreal-engine/API/Plugins/ModelContextProtocol)
- [MCP 规范](https://modelcontextprotocol.io/)
- [MCP Inspector](https://github.com/modelcontextprotocol/inspector)
