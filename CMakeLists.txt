cmake_minimum_required(VERSION 3.10)
project(tinytask C)

# 添加子目录
add_subdirectory(core)
add_subdirectory(example/basic) 
add_subdirectory(example/basic_event)
add_subdirectory(example/device_test)
add_subdirectory(example/lcd_simulator)