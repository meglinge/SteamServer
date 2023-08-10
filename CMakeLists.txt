# CMakeList.txt: Engine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(steamwork)



set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ThirdTool/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

#项目名称
project(Engine)

execute_process(
    COMMAND python ${CMAKE_SOURCE_DIR}/script/steamlib.py
)

# 将源代码添加到此项目的可执行文件。
aux_source_directory(./Sources/ DIR_SRCS)
add_executable(Engine ${DIR_SRCS})

# 导入外部依赖
find_package(Taskflow CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(doctest CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(rttr CONFIG REQUIRED)
target_link_libraries(Engine PUBLIC
    Taskflow::Taskflow
    Eigen3::Eigen
    doctest::doctest
    spdlog::spdlog
    RTTR::Core
    PUBLIC ${STEAMAPI_BINARY_DIR}
)
target_include_directories(${PROJECT_NAME}
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${STEAMAPI_SOURCE_DIR}
)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/steam_appid.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${STEAMAPI_SHARED_DIR}
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
) 
set_property(TARGET Engine PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded)

