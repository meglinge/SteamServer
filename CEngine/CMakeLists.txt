# CMakeList.txt: Engine 的 CMake 项目，在此处包括源代码并定义
cmake_minimum_required(VERSION 3.20)

# 项目名称
project(CEngine)

set(Vulkan_INCLUDE_DIR $ENV{VULKAN_SDK}/Include)
set(Vulkan_LIBRARY $ENV{VULKAN_SDK}/Lib/vulkan-1.lib)

# 将源代码添加到此项目的可执行文件。
aux_source_directory(./Sources/ DIR_SRCS)
add_executable(${PROJECT_NAME} ${DIR_SRCS})

find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)
find_package(mimalloc REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC
    Vulkan::Vulkan
    spdlog::spdlog
    glfw
    mimalloc
)

# target_include_directories(${PROJECT_NAME}
# PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
# PUBLIC ${STEAMAPI_SOURCE_DIR}
# )
set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded)
