cmake_minimum_required(VERSION 3.0)
project(PathTracer)

set(CMAKE_AUTOMOC ON)

include_directories(include)
set(HEADER
        include/ConstantHelper.h
        include/Point.h
        include/Triangle.h
        include/AABB.h
        include/BVH.h
        include/Mesh.h
        include/Scene.h
        include/Ray.h
        include/RenderWidget.h
)
aux_source_directory(src SOURCE)
add_executable(PathTracer
        ${HEADER}
        ${SOURCE}
)

# Assimp
find_package(assimp REQUIRED)

# Qt
set(QT_VERSION 5)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
target_link_libraries(
        PathTracer
        assimp
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)