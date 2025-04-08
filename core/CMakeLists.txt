file(GLOB SOURCES "*.c")

# 添加核心库
add_library(core ${SOURCES})

# 暴露头文件目录给依赖core的项目
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 定义接口，使用core的项目必须实现tt_task_port.h
target_compile_definitions(core PUBLIC CORE_REQUIRES_PORT_IMPLEMENTATION)
