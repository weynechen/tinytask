cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME stm32)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# TinyTask核心库和组件库路径
set(CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../core)
set(COMPONENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../component)

# TinyTask核心库
file(GLOB CORE_SOURCES "${CORE_DIR}/*.c")
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${CORE_SOURCES})

# TinyTask组件库 - 设备驱动
target_sources(${CMAKE_PROJECT_NAME} PRIVATE 
    ${COMPONENT_DIR}/tt_device.c
)

# TinyTask移植层和应用程序
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    port/tt_task_port.c
    include/key.c
    include/printf_uart.c
    key_task.c
    event_tasks.c
)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CORE_DIR}                         # TinyTask核心库
    ${COMPONENT_DIR}                    # TinyTask组件库
    ${CMAKE_CURRENT_SOURCE_DIR}/port    # 移植层
    ${CMAKE_CURRENT_SOURCE_DIR}/include # 应用层头文件
    ${CMAKE_CURRENT_SOURCE_DIR}         # 应用层
    Core/Inc                            # STM32 HAL 头文件
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
    STM32F103x8                         # 定义STM32芯片型号
    USE_HAL_DRIVER                      # 使用HAL库
)

# Add compile options to suppress warnings
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -Wno-implicit-fallthrough           # Disable implicit fallthrough warning
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
