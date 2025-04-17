#!/usr/bin/env python3

import os
import subprocess
import serial
import time
import sys
import threading
import argparse

# 配置参数
ELF_PATH = "build/stm32.elf"
BIN_PATH = "build/stm32.bin"
SERIAL_PORT = "/dev/ttyUSB0"
BAUDRATE = 115200
BUILD_DIR = "build"

def build_project():
    print("🔨 正在编译工程...")
    
    # 确保build目录存在
    if not os.path.exists(BUILD_DIR):
        os.makedirs(BUILD_DIR)
        
    # 进入build目录并执行cmake和make
    os.chdir(BUILD_DIR)
    
    # 执行cmake
    print("🔄 运行 cmake...")
    cmake_cmd = ["cmake", ".."]
    cmake_result = subprocess.run(cmake_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if cmake_result.returncode != 0:
        print("❌ cmake 失败:\n", cmake_result.stderr.decode())
        sys.exit(1)
    
    # 执行make
    print("🔄 运行 make...")
    make_cmd = ["make", "-j4"]
    make_result = subprocess.run(make_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if make_result.returncode != 0:
        print("❌ make 失败:\n", make_result.stderr.decode())
        sys.exit(1)
    
    # 返回上级目录
    os.chdir("..")
    
    print("✅ 编译完成")

def convert_elf_to_bin():
    print("🔄 正在将 ELF 转换为 BIN...")
    cmd = ["arm-none-eabi-objcopy", "-O", "binary", ELF_PATH, BIN_PATH]
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if result.returncode != 0:
        print("❌ 转换失败:\n", result.stderr.decode())
        sys.exit(1)
    print("✅ BIN 文件已生成:", BIN_PATH)

def enter_bootloader(port):
    print("🎯 正在控制串口进入 Bootloader 模式...")
    try:
        ser = serial.Serial(port)
        ser.setDTR(False)  # BOOT0 = 1
        ser.setRTS(True)   # RESET = 0
        time.sleep(0.1)
        ser.setRTS(False)  # RESET = 1
        time.sleep(0.2)
        ser.close()
        print("✅ 已进入 Bootloader")
    except Exception as e:
        print("❌ 串口控制失败:", e)
        sys.exit(1)

def flash_bin_with_stm32flash():
    print("🚀 正在烧录 BIN 文件...")
    cmd = [
        "stm32flash",
        "-b", str(BAUDRATE),
        "-w", BIN_PATH,
        "-v",
        "-g", "0x0",
        SERIAL_PORT
    ]
    process = subprocess.Popen(cmd)
    process.wait()
    if process.returncode != 0:
        print("❌ 烧录失败")
        sys.exit(1)
    print("✅ 烧录完成")

def reset_to_run(port):
    print("🔁 自动复位启动程序...")
    try:
        ser = serial.Serial(port)
        # BOOT0 = 0，RESET 拉低然后拉高
        ser.setDTR(False)   # BOOT0 = 0
        ser.setRTS(True)   # RESET = 0
        time.sleep(0.1)
        ser.setRTS(False)  # RESET = 1
        time.sleep(0.2)
        ser.close()
        print("✅ 已复位运行程序")
    except Exception as e:
        print("⚠️ 无法自动复位（可能没连好）：", e)

def monitor_serial(port, baudrate):
    print("\n📡 正在监听串口输出 (Ctrl+C 退出)...\n")
    try:
        ser = serial.Serial(port, baudrate, timeout=0.5 , rtscts=True)
        ser.setDTR(False)
        ser.setRTS(False)
        while True:
            data = ser.readline().decode(errors="ignore").strip()
            if data:
                print(data)
    except KeyboardInterrupt:
        print("\n🛑 监听已中止")
    except Exception as e:
        print("❌ 串口监听失败:", e)

def flash_firmware():
    if not os.path.exists(ELF_PATH):
        print(f"❌ 未找到 ELF 文件: {ELF_PATH}")
        sys.exit(1)

    convert_elf_to_bin()
    enter_bootloader(SERIAL_PORT)
    flash_bin_with_stm32flash()
    reset_to_run(SERIAL_PORT)

def parse_args():
    parser = argparse.ArgumentParser(description='STM32工程编译、烧录和监控工具')
    parser.add_argument('actions', nargs='+', choices=['build', 'flash', 'monitor'],
                      help='要执行的操作: build(编译), flash(烧录), monitor(监控)')
    return parser.parse_args()

def main():
    args = parse_args()
    
    # 执行指定的操作
    if 'build' in args.actions:
        build_project()
    
    if 'flash' in args.actions:
        flash_firmware()
    
    if 'monitor' in args.actions:
        monitor_serial(SERIAL_PORT, BAUDRATE)

if __name__ == "__main__":
    main()
