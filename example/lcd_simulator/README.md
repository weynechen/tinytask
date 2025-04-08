# LCD模拟器

这是一个基于SDL2的LCD显示器模拟器，用于测试嵌入式图形应用。模拟器提供了一个简单的接口，用于绘制像素点，并实时显示结果。

## 功能特性

- 模拟128x64像素的单色LCD显示屏
- 提供基本的像素绘制接口
- 使用SDL2实现跨平台支持（Windows, Linux, macOS）
- 支持实时更新和事件处理
- 支持多种颜色显示（黑、白、红、绿、蓝、黄、灰等）
- 内置按键事件处理（上、下、回车键）
- 支持Windows风格UI组件（按钮等）

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
- `void lcd_draw_pixel(uint16_t x, uint16_t y, uint32_t color)`: 绘制单个像素
- `void lcd_update(void)`: 更新显示内容
- `int lcd_get_event(void)`: 获取事件（如窗口关闭）
- `lcd_key_state_t lcd_get_key_state(void)`: 获取键盘按键状态
- `void lcd_deinit(void)`: 释放资源

## 颜色定义

LCD模拟器支持以下预定义颜色：

```c
#define COLOR_BLACK     0xFF000000  // 黑色
#define COLOR_WHITE     0xFFFFFFFF  // 白色
#define COLOR_RED       0xFFFF0000  // 红色
#define COLOR_GREEN     0xFF00FF00  // 绿色
#define COLOR_BLUE      0xFF0000FF  // 蓝色
#define COLOR_YELLOW    0xFFFFFF00  // 黄色
#define COLOR_GRAY      0xFFAAAAAA  // 灰色
#define COLOR_LIGHT_BLUE 0xFF8888FF // 淡蓝色
```

## 键盘控制

模拟器支持以下键盘控制：
- `u` 键：向上选择（对应KEY_UP_PRESSED）
- `d` 键：向下选择（对应KEY_DOWN_PRESSED）
- 回车键：确认选择（对应KEY_ENTER_PRESSED）

## 实例代码

示例代码展示了如何使用TinyTask库结合LCD模拟器创建交互式UI：

```c
// 初始化按钮
void init_buttons() {
    // 初始化按钮1
    buttons[0].x = 20;
    buttons[0].y = 10;
    buttons[0].width = 90;
    buttons[0].height = 22;
    buttons[0].text = "button1";
    buttons[0].selected = true;
    
    // 初始化按钮2
    buttons[1].x = 20;
    buttons[1].y = 40;
    buttons[1].width = 90;
    buttons[1].height = 22;
    buttons[1].text = "button2";
    buttons[1].selected = false;
}

// 显示任务
void display_task(void* arg) {
    TT_TASK_START;
    
    while (g_app_running) {
        // 清屏 - 设为全黑
        lcd_clear();
        
        // 绘制两个按钮
        draw_button(&buttons[0]);
        draw_button(&buttons[1]);
        
        // 更新显示
        lcd_update();
        
        // 延迟50ms
        TT_TASK_DELAY_MS(50);
    }
    
    TT_TASK_END;
}

// 在main函数中创建任务
tt_task_create(&display_task_obj, "display", display_task, NULL);
tt_task_create(&key_task_obj, "key", key_task, NULL);
```

示例将绘制一个Windows风格的UI界面，包含两个可选择的按钮，通过键盘可以控制选择和触发按钮事件。 