# FCMac 崩溃排查指南

## 问题：应用启动后立即崩溃

### 崩溃日志关键信息
```
Exception Type:    EXC_CRASH (SIGABRT)
Termination Reason:  Namespace SIGNAL, Code 6, Abort trap: 6

Thread 0 Crashed:
3   FCMac    Cartridge::read_from_file(...) + 740
4   FCMac    main + 116
```

### 原因分析

应用在启动时尝试加载 ROM 文件，但由于以下原因之一失败：

1. **ROM 文件不存在**（最常见）
   - 期望路径：`../Data/Super Mario Bros.nes`
   - 实际情况：Data 目录为空

2. **ROM 文件格式错误**
   - 文件不是有效的 NES ROM
   - 文件头不是 "NES\x1A"

3. **不支持的 Mapper**
   - FCMac 仅支持 Mapper 0/1/2
   - 你的 ROM 使用了其他 Mapper

### 解决方法

#### 方法 1：准备 ROM 文件（推荐）

```bash
# 1. 运行检查脚本
./check_rom.sh

# 2. 将你的 NES ROM 文件复制到 Data 目录
cp /path/to/your/game.nes Data/"Super Mario Bros.nes"

# 3. 再次运行检查脚本验证
./check_rom.sh

# 4. 如果通过，启动应用
open FCMac/FCMac.app
```

#### 方法 2：修改 ROM 路径

如果你的 ROM 文件名不同，修改 `FCMac/main.cpp` 第 22 行：

```cpp
// 原始代码
cartridge.read_from_file("../Data/Super Mario Bros.nes", 40976);

// 修改为你的 ROM 文件名
cartridge.read_from_file("../Data/YourGame.nes", file_size);
```

然后重新编译：
```bash
cd FCMac
make
```

#### 方法 3：使用测试 ROM

如果你没有 ROM 文件，可以使用公开的自制游戏（homebrew）进行测试：

```bash
# 下载测试 ROM（示例 - 需要你自己找合法的测试 ROM）
cd Data
# wget https://example.com/test-rom.nes
# mv test-rom.nes "Super Mario Bros.nes"
```

### ROM 文件要求

1. **必须是 NES/FC 格式**
   - 文件头必须是 "NES\x1A" (iNES 格式)
   - 不支持 NES 2.0 格式的特殊功能

2. **Mapper 支持**
   - ✅ Mapper 0 (NROM) - 简单游戏
   - ✅ Mapper 1 (MMC1) - 如《塞尔达传说》
   - ✅ Mapper 2 (UxROM) - 如《洛克人》
   - ❌ 其他 Mapper 不支持

3. **文件大小**
   - 典型大小：40KB - 512KB
   - 《超级马里奥兄弟》：40976 字节

### 使用检查脚本

我们提供了 `check_rom.sh` 脚本来验证 ROM 文件：

```bash
./check_rom.sh
```

脚本会检查：
- ✅ Data 目录是否存在
- ✅ ROM 文件是否存在
- ✅ 文件大小是否合理
- ✅ NES 文件头是否正确
- ✅ Mapper 类型是否支持

### 常见问题

**Q: 我没有 ROM 文件，在哪里获取？**

A: 由于版权原因，我们无法提供商业游戏 ROM。你可以：
- 使用自己拥有的游戏卡带备份
- 寻找公开的自制游戏（homebrew）
- 使用测试 ROM

**Q: 可以支持更多 Mapper 吗？**

A: 可以，但需要在 `Mapper/` 目录中实现新的 Mapper 类。每个 Mapper 的硬件行为都不同。

**Q: 为什么不用命令行参数传递 ROM 路径？**

A: 这是原始代码的设计。你可以修改 `main.cpp` 来支持命令行参数。

### 成功运行的标志

当 ROM 正确加载后，你应该看到：
1. Qt 窗口打开
2. 游戏画面渲染（256×240 像素）
3. 音频开始播放
4. 键盘输入响应（WASD/JK）

如果仍然崩溃，请查看控制台输出的错误信息。
