# LCD模拟器

这是一个基于SDL2的LCD显示器模拟器，用于测试嵌入式图形应用。模拟器提供了一个简单的接口，用于绘制像素点，并实时显示结果。

## 功能特性

- 模拟128x64像素的单色LCD显示屏
- 提供基本的像素绘制接口
- 使用SDL2实现跨平台支持（Windows, Linux, macOS）
- 支持实时更新和事件处理

## 依赖项

- CMake 3.10+
- SDL2 开发库
- C编译器（GCC, Clang等）

## 安装依赖

### Ubuntu/Debian:
```bash
sudo apt-get install libsdl2-dev
```

### Fedora/RHEL:
```bash
sudo dnf install SDL2-devel
```

### macOS (使用Homebrew):
```bash
brew install sdl2
```

### Windows:
下载并安装SDL2开发库 (https://www.libsdl.org/download-2.0.php)

## 编译与运行

1. 确保你已经安装了所需的依赖项
2. 构建TinyTask项目
   ```bash
   mkdir -p build
   cd build
   cmake ..
   make
   ```
3. 运行LCD模拟器示例
   ```bash
   ./example/lcd_simulator/lcd_simulator
   ```

如果安装出现问题，请参考：https://wiki.libsdl.org/SDL2/Installation

## 接口说明

LCD模拟器提供以下接口：

- `int lcd_init(void)`: 初始化LCD模拟器
- `void lcd_clear(void)`: 清空屏幕
- `void lcd_draw_pixel(uint16_t x, uint16_t y, lcd_pixel_state_t state)`: 绘制单个像素
- `void lcd_update(void)`: 更新显示内容
- `int lcd_get_event(void)`: 获取事件（如窗口关闭）
- `void lcd_deinit(void)`: 释放资源

## 实例代码

示例代码展示了如何使用TinyTask库结合LCD模拟器创建动画效果：

```c
// 创建定时任务，每50ms更新一次显示
tinytask_create("update_display", update_display, &sine_anim, 50, 1);
```

示例将绘制一个正弦波动画和边框。 