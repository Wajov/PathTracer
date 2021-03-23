cmake_minimum_required(VERSION 3.0)
project(PathTracer)

set(CMAKE_AUTOMOC ON)

include_directories(include)
set(HEADER
        include/ConstantHelper.h
        include/SampleHelper.h
        include/Point.h
        include/Triangle.h
        include/AABB.h
        include/BVH.h
        include/Material.h
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

# OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
target_link_libraries(
        PathTracer
        assimp
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)