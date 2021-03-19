cmake_minimum_required(VERSION 3.0)
project(PathTracer)

set(CMAKE_AUTOMOC ON)

include_directories(include)
set(HEADER
        include/Scene.h
)
aux_source_directory(src SOURCE)
add_executable(PathTracer
        ${HEADER}
        ${SOURCE}
)

# Assimp
find_package(assimp REQUIRED)

# tinyobjloadrer
find_package(tinyobjloader REQUIRED)

# Qt
set(QT_VERSION 5)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
target_link_libraries(
        PathTracer
        assimp
        tinyobjloader
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)