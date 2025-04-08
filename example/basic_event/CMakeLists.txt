cmake_minimum_required(VERSION 3.10)
project(tinytask_basic_event_example C)

# 设置C标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 核心库路径
set(CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../core)

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
add_library(tt_port_event STATIC port/tt_task_port.c ${OS_SPECIFIC_SRC})
target_include_directories(tt_port_event PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/port
    ${CORE_DIR}  # 需要访问core中的接口定义
)

# 添加例子程序
add_executable(basic_event event.c)

# 包含目录
target_include_directories(basic_event PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/port
    ${CORE_DIR}
)

# 链接库 - 先链接core再链接port
target_link_libraries(basic_event PRIVATE core tt_port_event ${PLATFORM_LIBS})
