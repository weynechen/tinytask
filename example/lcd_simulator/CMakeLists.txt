cmake_minimum_required(VERSION 3.10)
project(lcd_simulator C)

# 设置C标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 核心库路径
set(CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../core)

# 创建端口目录
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/port)

# 复制端口实现文件
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/port/tt_task_port.c)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../example/basic/port/tt_task_port.c 
         DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/port)
endif()

# 检测操作系统
if(WIN32)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/port/windows_tick.c)
        file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../example/basic/port/windows_tick.c 
             DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/port)
    endif()
    set(OS_SPECIFIC_SRC port/windows_tick.c)
    add_definitions(-DPLATFORM_WINDOWS)
elseif(UNIX)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/port/linux_tick.c)
        file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../example/basic/port/linux_tick.c 
             DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/port)
    endif()
    set(OS_SPECIFIC_SRC port/linux_tick.c)
    add_definitions(-DPLATFORM_LINUX)
    # Linux需要链接pthread库
    set(PLATFORM_LIBS pthread rt)
endif()

# 查找SDL2包
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# 创建移植层库
add_library(lcd_port STATIC port/tt_task_port.c ${OS_SPECIFIC_SRC})
target_include_directories(lcd_port PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/port
    ${CORE_DIR}  # 需要访问core中的接口定义
)

# 添加LCD模拟器程序
add_executable(lcd_simulator main.c lcd_driver.c font.c)

# 包含目录
target_include_directories(lcd_simulator PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/port
    ${CORE_DIR}
    ${SDL2_INCLUDE_DIRS}
)

# 链接库
target_link_libraries(lcd_simulator 
    PRIVATE 
    core 
    lcd_port 
    ${SDL2_LIBRARIES} 
    ${PLATFORM_LIBS}
    m  # 数学库，用于sin函数
) 