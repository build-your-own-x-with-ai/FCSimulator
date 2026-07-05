#!/bin/bash

# FCMac ROM 文件检查脚本

echo "========================================"
echo "FCMac ROM 文件检查"
echo "========================================"
echo ""

DATA_DIR="/Users/i/Code/i/NES/FCSimulator/Data"
ROM_FILE="$DATA_DIR/Super Mario Bros.nes"

# 检查 Data 目录
if [ ! -d "$DATA_DIR" ]; then
    echo "❌ Data 目录不存在"
    echo "   创建目录: mkdir -p $DATA_DIR"
    exit 1
fi

echo "✅ Data 目录存在: $DATA_DIR"
echo ""

# 检查 ROM 文件
if [ ! -f "$ROM_FILE" ]; then
    echo "❌ ROM 文件不存在: Super Mario Bros.nes"
    echo ""
    echo "解决方法："
    echo "1. 将 NES ROM 文件复制到 Data 目录"
    echo "   cp /path/to/your/rom.nes \"$ROM_FILE\""
    echo ""
    echo "2. 或者修改 FCMac/main.cpp 中的 ROM 路径"
    echo "   当前路径: ../Data/Super Mario Bros.nes"
    echo ""
    echo "Data 目录中现有文件："
    ls -la "$DATA_DIR"
    exit 1
fi

echo "✅ ROM 文件存在"
echo ""

# 检查文件大小
FILE_SIZE=$(stat -f%z "$ROM_FILE")
echo "📦 文件大小: $FILE_SIZE 字节"

if [ $FILE_SIZE -lt 16 ]; then
    echo "❌ 文件太小，可能不是有效的 NES ROM"
    exit 1
fi

# 检查 NES 头部
HEADER=$(xxd -l 4 -p "$ROM_FILE")
EXPECTED="4e45531a"  # "NES\x1A"

if [ "$HEADER" = "$EXPECTED" ]; then
    echo "✅ NES 文件头正确"

    # 读取 Mapper 信息
    MAPPER_LOW=$(xxd -s 6 -l 1 -p "$ROM_FILE" | sed 's/^/0x/')
    MAPPER_HIGH=$(xxd -s 7 -l 1 -p "$ROM_FILE" | sed 's/^/0x/')
    MAPPER=$(printf "%d" $(( (($MAPPER_HIGH & 0xF0) | ($MAPPER_LOW >> 4)) )))

    echo "📋 Mapper 类型: $MAPPER"

    case $MAPPER in
        0)
            echo "   ✅ Mapper 0 (NROM) - 支持"
            ;;
        1)
            echo "   ✅ Mapper 1 (MMC1) - 支持"
            ;;
        2)
            echo "   ✅ Mapper 2 (UxROM) - 支持"
            ;;
        *)
            echo "   ❌ Mapper $MAPPER - 不支持！"
            echo "   FCMac 仅支持 Mapper 0/1/2"
            exit 1
            ;;
    esac
else
    echo "❌ 文件头错误，不是有效的 NES ROM"
    echo "   期望: NES\\x1A"
    echo "   实际: $HEADER"
    exit 1
fi

echo ""
echo "========================================"
echo "✅ ROM 文件检查通过，可以运行 FCMac"
echo "========================================"
echo ""
echo "运行命令："
echo "  open /Users/i/Code/i/NES/FCSimulator/FCMac/FCMac.app"
