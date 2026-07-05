# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a Nintendo Entertainment System (FC/NES) emulator with three platform implementations:
- **FCLnx**: Linux version with CPU emulation, PPU rendering, APU audio playback (via OpenAL)
- **FCMac**: macOS version with CPU emulation, PPU rendering, APU audio playback (via OpenAL framework)
- **FCWin**: Windows version with CPU emulation and PPU rendering (no audio)

All are Qt-based GUI applications written in C++.

## Architecture

The emulator follows the classic NES architecture with separate components:

### Core Components
- **CPU** (`cpu.h/cpp`): 6502 CPU emulation with cycle-accurate execution
- **PPU** (`ppu.h/cpp`): Picture Processing Unit for graphics rendering (256×240 resolution)
- **APU** (`apu.h/cpp`, Linux only): Audio Processing Unit for sound generation
- **CpuBus** (`cpu_ram.h/cpp`): CPU memory bus and RAM
- **PpuBus** (`ppu_bus.h/cpp`): PPU memory bus for pattern tables and nametables
- **Controller** (`controller.h/cpp`): NES controller input handling
- **Cartridge** (`cartridge.h/cpp`): ROM file loading and parsing

### Mapper System
The `Mapper/` directory contains cartridge mapper implementations (memory bank switching):
- `mapper_base.h`: Abstract base class defining the mapper interface
- `mapper_0.h/cpp`: NROM mapper (no bank switching)
- `mapper_1.h/cpp`: MMC1 mapper
- `mapper_2.h/cpp`: UxROM mapper

Each mapper handles:
- CPU reads/writes to PRG ROM/RAM (0x6000-0xFFFF)
- PPU reads/writes to CHR ROM/RAM (pattern tables)
- Nametable mirroring configuration

### Global State
`total.h` declares all global instances used throughout the emulator. All major components (CPU, PPU, APU, buses, controllers, cartridge) are instantiated as globals in `main.cpp`.

## Building

All versions use Qt Creator project files (`.pro`):

**Linux version:**
```bash
cd FCLnx
qmake FCLnxNew.pro
make
```

The Linux build requires OpenAL development libraries (`-lopenal`).

**macOS version:**
```bash
cd FCMac
qmake FCMac.pro
make
```

The macOS build uses the system OpenAL framework (`-framework OpenAL`). On Qt 6.11+, the build includes `-Wno-error=implicit-function-declaration` to work around a Qt header issue on arm64.

**Windows version:**
```bash
cd FCWin
qmake FCWin.pro
nmake  # or mingw32-make
```

## Running

ROM files are loaded via hardcoded paths in `main.cpp`. All versions expect:
```cpp
cartridge.read_from_file("../Data/Super Mario Bros.nes", 40976);
```

You need to:
1. Create a `Data/` directory at the repository root
2. Place NES ROM files there (e.g., `Super Mario Bros.nes`)
3. Update the path in `main.cpp` if needed

The emulator runs at ~60 FPS (16590-16667 μs per frame).

**Platform-specific notes:**
- **Linux/macOS**: Full audio support via OpenAL
- **Windows**: No audio (APU code not included)
- **macOS**: OpenAL is deprecated since 10.15 but still functional; warnings can be ignored

## Timing and Execution

The emulation loop in `MainWindow::OnNewFrame()` follows NES timing:
- 3 PPU cycles per 1 CPU cycle
- APU runs every 65 scanlines (4 times per frame)
- Each frame completes when PPU signals `frame_finished`

Audio buffering (Linux only) maintains 3-13 queued buffers to prevent stuttering.

## Key Implementation Details

- **PPU rendering**: Outputs to `Ppu2.frame_data[256][240][3]` RGB array
- **Controller input**: Qt keyboard events mapped to NES controller buttons via `controller.key_map`
- **Nametable mirroring**: Handled by mapper instances via `nametable_mirror` field
- **Address decoding**: CPU/PPU memory access routes through bus classes which delegate to mapper when needed

When adding new mappers, inherit from `Mapper` base class and implement all virtual methods for CPU/PPU memory access.
