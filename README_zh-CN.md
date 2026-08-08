# TextReaderOverlay-NX Revived（中文说明）

这是 [diwo/TextReaderOverlay-NX](https://github.com/diwo/TextReaderOverlay-NX) 的兼容性维护分支。它是一个运行在 Nintendo Switch Atmosphere/Tesla Menu 上的文本阅读 Overlay，可以在游戏或其他前台程序上方浏览 SD 卡中的 `.txt` 文件。

## 为什么要修改

原项目的功能设计很实用，但原始代码依赖较旧的 libnx、libtesla 和 HID 接口。随着 Atmosphere 和开发工具链更新，旧版会出现插件不支持、Browser 崩溃、无法打开 `sdmc:`、打开文本后一片空白等问题。

本分支不是只做“能编译”的修改，而是在真实 Switch 上结合 crash report 和逐版实机测试完成：

- 更新旧版 HID API 和 libtesla 接口；
- 修复旧版字体库导致的内存不足；
- 启动时显式挂载 SD 卡，退出时卸载；
- 修复目录浏览和路径处理；
- 修复新版 libtesla 下根级 `CustomDrawer` 尺寸为 0 导致的文字全部被裁剪；
- 增加 UTF-8、UTF-16、UTF-32、GB18030/GBK、Windows-1252 自动检测；
- 使用 Switch 系统共享字体显示中日韩文字，不再携带大型 CJK 字体；
- 修复短文本末尾出现无关堆内存内容的问题；
- 修复 Help 页面在新版 libtesla 下的裁剪问题；
- 增加可选的自动换行功能，方便阅读超长文本行；
- 限制字形位图缓存，修复反复放大字号时可能出现的 ovlloader 崩溃。

## 使用条件

- 已安装 Atmosphere 的 Nintendo Switch；
- 最新版 `nx-ovlloader`；
- 最新版 Tesla Menu；
- SD 卡中存在扩展名为 `.txt` 的文本文件。

目前已在 Atmosphere 1.11.2 / 系统版本 22.5.0 环境中完成实机测试，其他版本不作绝对保证。

## 安装

1. 将最新版 [nx-ovlloader](https://github.com/WerWolv/nx-ovlloader) 安装到 SD 卡；
2. 将最新版 [Tesla Menu](https://github.com/WerWolv/Tesla-Menu) 安装到 SD 卡；
3. 从本仓库 [Releases](https://github.com/chiya695/TextReaderOverlay-NX-Revived/releases/latest) 下载 ZIP，并解压到 SD 卡根目录；
4. 将 `.txt` 文件放到 SD 卡任意位置；
5. 安装或替换 Overlay 后建议完整重启 Switch；
6. 使用 `L + 十字键下 + 右摇杆按下` 打开 Tesla Menu；
7. 选择 `Text Reader`，再进入 `Browse...` 选择文本；
8. 在主菜单进入 `Settings...`，可以启用或关闭自动换行。

如果替换后仍像旧版本，删除 `sd:/switch/.overlays/TextReaderOverlay-NX.ovl`，重新复制新版本，然后重启 Switch。仅退出 Tesla 或隐藏 Overlay，不一定会让已经驻留内存的旧代码消失。

## 按键

### 文件浏览

- `A`：打开目录或文本；
- `Y`：添加/取消 Favorite；
- `B`：返回上一级。

### 阅读器

- 左摇杆：滚动；
- `ZL + 左摇杆`：加速滚动；
- `ZR + 左摇杆`：快速滚动，或跳到顶部/底部；
- 右摇杆上下：细微滚动；
- 右摇杆左右：关闭自动换行时左右平移；
- 按下右摇杆：关闭自动换行时重置水平位置；
- 十字键上下：调整字体大小；
- `Y`：切换书签；
- `L/R`：上一个/下一个书签；
- `X`：隐藏 Overlay；
- `B`：关闭阅读器；
- `-`：显示/隐藏诊断信息。

## 设置

`Settings...` 中提供 `Automatic Line Wrapping` 开关，默认开启。开启后，超长文本行会按照屏幕宽度拆成多个视觉行，阅读时不需要长距离左右平移；关闭后恢复原来的水平平移方式。设置会保存到 Overlay 配置中，并在下次打开文本时生效。

## 文本编码支持

程序按流式分块方式读取，不会把整本大文件一次性加载到内存，支持：

- UTF-8（有或无 BOM）；
- UTF-16 LE/BE；
- UTF-32 LE/BE；
- GB18030/GBK；
- Windows-1252。

文字使用 Switch 系统共享字体渲染，支持拉丁文、日文、简体中文、繁体中文和韩文，并避免在 Overlay 中携带大型中文字库。

## 原始出处与许可证

- 原始项目：[diwo/TextReaderOverlay-NX](https://github.com/diwo/TextReaderOverlay-NX)；
- Overlay 框架：[WerWolv/libtesla](https://github.com/WerWolv/libtesla)；
- Overlay 加载器：[WerWolv/nx-ovlloader](https://github.com/WerWolv/nx-ovlloader)；
- Tesla Menu：[WerWolv/Tesla-Menu](https://github.com/WerWolv/Tesla-Menu)；
- Atmosphere：[Atmosphere-NX/Atmosphere](https://github.com/Atmosphere-NX/Atmosphere)。

本仓库保留原项目 GPL 许可证，请同时遵守 vendored 依赖各自的许可证要求，详见 [LICENSE](LICENSE)。

这是非官方社区维护分支，与 Nintendo、Atmosphere、Tesla Menu 及原作者没有隶属关系。
