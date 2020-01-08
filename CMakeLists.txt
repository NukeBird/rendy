cmake_minimum_required(VERSION 3.7)

include("cmake/HunterGate.cmake")

HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.228.tar.gz"
    SHA1 "0546cabd20ed784a2245083eb59e9185bf1d63a9"
)

project(Rendy)

file(GLOB_RECURSE PROJECT_SOURCE_FILES
    "engine/*.h"
    "engine/*.hpp"
    "engine/*.cpp"
)

hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)

hunter_add_package(stb)
find_package(stb CONFIG REQUIRED)

#hunter_add_package(Assimp)
#find_package(Assimp CONFIG REQUIRED)

hunter_add_package(spdlog)
find_package(spdlog CONFIG REQUIRED)

hunter_add_package(glm)
find_package(glm REQUIRED)

add_subdirectory(dep)
set(PROJECT_LIBS SDL2::SDL2main SDL2::SDL2 stb::stb
    assimp glm gli spdlog::spdlog)

if(WIN32)
    hunter_add_package(glew)
    find_package(glew CONFIG REQUIRED)

    set(PROJECT_LIBS ${PROJECT_LIBS} glew::glew)

    add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

    foreach(_source IN ITEMS ${PROJECT_SOURCE_FILES})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
        string(REPLACE "/" "\\" _group_path "${_group_path}")
        source_group("${_group_path}" FILES "${_source}")
    endforeach()
else()
    #TODO: OPTICK_ENABLED=OFF
    add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
endif()

set(PROJECT_LIBS ${PROJECT_LIBS} OptickCore)

target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS})