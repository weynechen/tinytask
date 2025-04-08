cmake_minimum_required(VERSION 3.10)
project(tinytask_device_test_example C)

# 设置C标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 核心库路径
set(CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../core)
set(COMPONENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../component)

# 检测操作系统
if(WIN32)
    set(OS_SPECIFIC_SRC port/windows_tick.c)
    add_definitions(-DPLATFORM_WINDOWS)
elseif(UNIX)
    set(OS_SPECIFIC_SRC port/linux_tick.c)
    add_definitions(-DPLATFORM_LINUX)
    # Linux需要链接pthread库
    set(PLATFORM_LIBS pthread rt)
endif()

# 创建移植层库
add_library(tt_port_device_test STATIC 
    port/tt_task_port.c 
    ${OS_SPECIFIC_SRC}
)
target_include_directories(tt_port_device_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/port
    ${CORE_DIR}  # 需要访问core中的接口定义
)

# 添加设备树库
add_library(tt_device STATIC
    ${COMPONENT_DIR}/tt_device.c
)
target_include_directories(tt_device PUBLIC
    ${COMPONENT_DIR}
    ${CORE_DIR}
)

# 键盘设备库
add_library(key_device STATIC
    include/key.c
    key_task.c
    event_tasks.c
)
target_include_directories(key_device PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CORE_DIR}
    ${COMPONENT_DIR}
)

# 添加例子程序
add_executable(device_test device_test.c)

# 包含目录
target_include_directories(device_test PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/port
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CORE_DIR}
    ${COMPONENT_DIR}
)

# 链接库
target_link_libraries(device_test PRIVATE 
    core 
    tt_port_device_test 
    key_device
    tt_device
    ${PLATFORM_LIBS}
) 