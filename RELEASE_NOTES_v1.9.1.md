# TextReaderOverlay-NX Revived v1.9.1

## English

This maintenance release fixes a crash found during real-world use of v1.9. When a multilingual text file was displayed at progressively larger font sizes, libtesla retained bitmap data for every character and font-size combination without a memory limit. Automatic wrapping also measures glyphs, which made the unbounded cache grow faster on long Chinese text.

### Fixes

- Limits cached glyph bitmap data to approximately 512 KiB.
- Frees old stb_truetype bitmap allocations before clearing the cache.
- Keeps automatic wrapping, multilingual system-font fallback, and the full font-size range available.
- Prevents glyph data from accumulating indefinitely across repeated font-size changes.

The repaired build was tested on real Switch hardware by repeatedly increasing and decreasing the reader font size and scrolling multilingual text. The previously reproducible Atmosphere/ovlloader crash no longer occurred.

### Installation

Extract `TextReaderOverlay-NX-1.9.1.zip` to the SD-card root, replacing `sd:/switch/.overlays/TextReaderOverlay-NX.ovl`. Fully reboot the Switch after replacing an already loaded overlay; simply closing Tesla Menu may leave the previous version resident in memory.

### Attribution

This remains a maintained compatibility fork of [diwo/TextReaderOverlay-NX](https://github.com/diwo/TextReaderOverlay-NX). The original project, concept, and GPL licensing remain credited and preserved.

---

## 中文

这是针对 v1.9 实际使用中发现的崩溃问题所发布的维护版本。当包含大量中日韩字符的文本被逐级放大字号时，libtesla 会为每个“字符＋字号”生成字形位图，但原有缓存没有内存上限。自动换行功能也需要测量字形宽度，因此在较长的中文文本中会让缓存增长得更快，最终可能导致 ovlloader 崩溃并触发 Atmosphere 错误页面。

### 修复内容

- 将字形位图缓存限制在约 512 KiB。
- 清理缓存前正确释放 stb_truetype 分配的旧字形位图。
- 保留自动换行、多语言系统字体回退和完整字号调整范围。
- 避免多次放大、缩小字号后字形数据持续累积。

修复版本已经在真实 Switch 上完成实机验证，包括反复放大和缩小字号、滚动中日韩文本等操作。此前能够复现的 Atmosphere/ovlloader 崩溃没有再次出现。

### 安装方法

将 `TextReaderOverlay-NX-1.9.1.zip` 解压到 SD 卡根目录，替换 `sd:/switch/.overlays/TextReaderOverlay-NX.ovl`。替换已经加载过的 Overlay 后请完整重启 Switch；仅关闭 Tesla Menu 可能仍会继续使用驻留在内存中的旧版本。

### 原始出处

本项目仍是 [diwo/TextReaderOverlay-NX](https://github.com/diwo/TextReaderOverlay-NX) 的兼容性维护分支，保留并明确标注原项目、设计思路和 GPL 许可证信息。
