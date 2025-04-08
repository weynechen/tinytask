file(GLOB SOURCES "*.c")

# Add a library with the above sources
add_library(component ${SOURCES})
target_include_directories(component PRIVATE ${PROJECT_SOURCE_DIR}/port)
target_include_directories(component PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../core)
