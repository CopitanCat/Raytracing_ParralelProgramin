cmake_minimum_required(VERSION 4.3)

project(RayTracing)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Raylib
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)

FetchContent_MakeAvailable(raylib)

# OpenMP
find_package(OpenMP REQUIRED)

add_executable(RayTracing
        main.cpp
        Vector.h
        Surface.h
        Material.h
        Scene.h
        RaytracingLogic.h
        Intersection.h
        Intersection.h
        Camera.h
        Dependencies.h
)

target_link_libraries(RayTracing PRIVATE
        raylib
        OpenMP::OpenMP_CXX
)
