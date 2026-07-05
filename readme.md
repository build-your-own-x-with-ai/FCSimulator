## 简单的FC模拟器

---

一共有 Windows、Linux 和 macOS 三个版本的 FC 模拟器源码。

- **Windows 版本**：包含了游戏运行和画面显示的功能（无音频）
- **Linux 版本**：包含了游戏运行、画面显示和声音播放的功能（完整）
- **macOS 版本**：包含了游戏运行、画面显示和声音播放的功能（完整）

## 快速开始

### 1. 选择你的平台

```bash
cd FCLnx   # Linux 版本
cd FCMac   # macOS 版本
cd FCWin   # Windows 版本
```

### 2. 准备 ROM 文件

```bash
# 在仓库根目录创建 Data 文件夹并放入 ROM
mkdir -p Data
cp your-game.nes Data/"Super Mario Bros.nes"
```

### 3. 构建和运行

**macOS**:
```bash
cd FCMac
qmake FCMac.pro && make
open FCMac.app
```

**Linux**:
```bash
cd FCLnx
qmake FCLnxNew.pro && make
./Step_06
```

**Windows**:
```bash
cd FCWin
qmake FCWin.pro && nmake
FCWin.exe
```

## 功能特性

- ✅ 6502 CPU 模拟
- ✅ PPU 图形渲染（256×240）
- ✅ APU 音频播放（Linux/macOS）
- ✅ 支持 Mapper 0/1/2
- ✅ 键盘输入（WASD/JK/空格/Shift）

## 文档

- [`CLAUDE.md`](CLAUDE.md) - 开发者文档
- [`FCMac/README.md`](FCMac/README.md) - macOS 版本说明
- [`check_rom.sh`](check_rom.sh) - ROM 文件检查工具

## 许可证

原始项目
