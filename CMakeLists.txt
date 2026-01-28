cmake_minimum_required(VERSION 3.16)
project(SunnyEngine VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Critical for clangd
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")


find_package(SFML 3.0.2 COMPONENTS System Window Graphics Audio REQUIRED)
find_package(SunnyLog REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad include/glad/src/glad.c)
target_include_directories(glad PUBLIC include/glad/include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.c")

add_executable(bin 
    ${SOURCES}
)

target_link_libraries(bin PRIVATE
    glad
    OpenGL::GL
    SunnyLog::SunnyLog
    SFML::System
    SFML::Window
    SFML::Graphics
    SFML::Audio
)

target_include_directories(bin PRIVATE
    ${SOURCE_DIR} 
    include
)
