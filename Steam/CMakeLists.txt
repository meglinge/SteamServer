# CMakeList.txt: Engine 的 CMake 项目，在此处包括源代码并定义
cmake_minimum_required(VERSION 3.20)

execute_process(
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/script/steamlib.py -d ${CMAKE_CURRENT_BINARY_DIR}
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

if(WIN32)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")
endif()

include(steamwork)

# 项目名称
project(Steam)

# 将源代码添加到此项目的可执行文件。
aux_source_directory(./Sources/ DIR_SRCS)
add_executable(Steam ${DIR_SRCS})

# 导入外部依赖
find_package(Taskflow CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(doctest CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(rttr CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
target_link_libraries(Steam PUBLIC
    Taskflow::Taskflow
    Eigen3::Eigen
    doctest::doctest
    spdlog::spdlog
    RTTR::Core
    glm::glm
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
set_property(TARGET Steam PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded)
