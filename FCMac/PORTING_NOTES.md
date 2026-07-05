# macOS 版本移植总结

## 完成的工作

### 1. 创建 FCMac 目录
- 从 FCLnx 复制所有源代码
- 保留完整的 APU 音频支持

### 2. 修改的文件

#### FCMac/mainwindow.h
- 添加 macOS/Linux 条件编译
- OpenAL 头文件路径：`<OpenAL/al.h>` 和 `<OpenAL/alc.h>`

#### FCMac/FCMac.pro
- 修改目标名称为 `FCMac`
- 使用 `-framework OpenAL` 替代 `-lopenal`
- 添加 `QMAKE_MACOSX_DEPLOYMENT_TARGET = 11.0`
- 添加 `-Wno-error=implicit-function-declaration` 以解决 Qt 6.11 的警告

#### 批量修改
- 将所有 `endl` 替换为 `Qt::endl`（Qt 6 要求）

### 3. 构建结果

✅ 编译成功  
✅ 可执行文件生成：`FCMac.app/Contents/MacOS/FCMac` (150KB)  
✅ 所有功能模块完整（CPU、PPU、APU、Mapper 0/1/2）  

### 4. 警告信息（可忽略）

- OpenAL API 弃用警告（macOS 10.15+）
- Qt 6.11 `__yield` 隐式声明警告
- 部分未使用参数警告

## 技术细节

### OpenAL 支持
- macOS 系统自带 OpenAL.framework
- 位置：`/System/Library/Frameworks/OpenAL.framework`
- 虽然已弃用，但在当前 macOS 版本仍可正常使用

### 跨平台兼容性
```cpp
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
```

### Qt 6 兼容性
- `endl` → `Qt::endl`
- 需要 `-Wno-error=implicit-function-declaration` 标志

## 使用说明

### 运行前准备
1. 在仓库根目录创建 `Data/` 文件夹
2. 放入 NES ROM 文件（如 `Super Mario Bros.nes`）
3. 运行：`open FCMac.app`

### 键盘控制
- **WASD**: 方向键
- **JK**: B/A 按钮
- **空格/Shift**: START/SELECT

## 与原版本的差异

### Linux 版（FCLnx）
- OpenAL 通过 `-lopenal` 链接
- 头文件路径：`<AL/al.h>`

### macOS 版（FCMac）
- OpenAL 通过 `-framework OpenAL` 链接
- 头文件路径：`<OpenAL/al.h>`
- 额外的编译器标志处理 Qt 6.11 兼容性

### Windows 版（FCWin）
- 无 APU 音频支持
- 使用 QTimer 而非 QThread 进行帧同步

## 验证清单

- [x] 源代码编译通过
- [x] 可执行文件生成
- [x] OpenAL 框架正确链接
- [x] Qt 6 兼容性问题解决
- [x] 文档完善（README.md、CLAUDE.md）
- [ ] 实际 ROM 运行测试（需要用户提供 ROM 文件）

## 下一步

要完整验证移植成功，需要：
1. 将 NES ROM 文件放入 `Data/` 目录
2. 运行 `FCMac.app`
3. 测试图形渲染、音频播放、键盘输入

如果出现问题，请检查：
- ROM 文件路径是否正确
- Qt 6 是否正确安装
- 系统 OpenAL 框架是否可用
