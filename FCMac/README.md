# FCMac - NES Emulator for macOS

macOS 版本的 FC/NES 模拟器，移植自 FCLnx（Linux 版本）。

## 功能特性

- ✅ CPU 模拟（6502 处理器）
- ✅ PPU 图形渲染（256×240 分辨率）
- ✅ APU 音频播放（通过 OpenAL）
- ✅ 手柄输入（键盘映射）
- ✅ 支持 Mapper 0/1/2

## 构建要求

- macOS 11.0 或更高版本
- Qt 6.x（已在 Qt 6.11.0 上测试）
- Xcode 命令行工具
- 系统自带 OpenAL.framework

## 构建步骤

```bash
# 确保你在 FCMac 目录
cd FCMac

# 使用 qmake 生成 Makefile
qmake FCMac.pro

# 编译
make

# 运行（需要先准备 ROM 文件）
open FCMac.app
```

## 准备 ROM 文件

1. 在仓库根目录创建 `Data/` 文件夹：
   ```bash
   cd /Users/i/Code/i/NES/FCSimulator
   mkdir -p Data
   ```

2. 将 NES ROM 文件（如 `Super Mario Bros.nes`）放入 `Data/` 目录

3. 默认加载路径为 `../Data/Super Mario Bros.nes`，可以在 `main.cpp` 中修改

## 键盘映射

### 左手柄（P1）
- **方向键**: W/A/S/D
- **A 按钮**: K
- **B 按钮**: J
- **START**: 空格
- **SELECT**: Shift

### 右手柄（P2）
- **方向键**: L/逗号/句号/斜杠
- **A 按钮**: X
- **B 按钮**: Z
- **START**: 左方括号 [
- **SELECT**: 右方括号 ]

## 已知问题

- OpenAL 在 macOS 10.15+ 已被标记为弃用，但仍然可以正常使用
- 编译时会显示 OpenAL 弃用警告，可以忽略
- Qt 6.11 在 arm64 上有一个关于 `__yield` 的警告，已通过编译选项禁用

## 与 Linux 版本的区别

唯一的主要区别是 OpenAL 头文件路径：

**Linux**: `#include <AL/al.h>`  
**macOS**: `#include <OpenAL/al.h>`

代码中使用条件编译 `#ifdef __APPLE__` 来处理这个差异。

## 性能

- 目标帧率: 60 FPS
- 每帧时间: ~16.59 ms
- 音频采样率: 44100 Hz
- 音频缓冲: 3-13 个缓冲区队列

## 许可证

与原项目相同
