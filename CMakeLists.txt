# CMakeList.txt: Engine 的 CMake 项目，在此处包括源代码并定义
cmake_minimum_required(VERSION 3.20)

option(ENABLE_MANIFEST_MODE "Enable Manifest Mode" ON)
set(ENABLE_MANIFEST_MODE OFF)
# # 根据选项的值来控制某条指令的开和关
if(ENABLE_MANIFEST_MODE)
    set(VCPKG_MANIFEST_MODE ON)
    set(Z_VCPKG_CHECK_MANIFEST_MODE ON)
else()
    set(VCPKG_MANIFEST_MODE OFF)
    set(Z_VCPKG_CHECK_MANIFEST_MODE OFF)
    execute_process(
        COMMAND python ${CMAKE_SOURCE_DIR}/script/vcpkg.py -d ${CMAKE_SOURCE_DIR}
    )
endif()
# C的解决办法
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fexec-charset=UTF-8")
# C++ 的解决办法
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexec-charset=UTF-8")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ThirdTool/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
if(WIN32)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")

endif()


# 项目名称
project(Engine)

add_subdirectory(CEngine)
add_subdirectory(Steam)