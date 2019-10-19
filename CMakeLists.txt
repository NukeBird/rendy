cmake_minimum_required(VERSION 3.7)

include("cmake/HunterGate.cmake")

HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.220.tar.gz"
    SHA1 "c2296fdc5ad3b1085a2ea985600f36d5a1351b93"
)

project(Rendy)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE PROJECT_SOURCE_FILES
    "src/*.h"
    "src/*.hpp"
    "src/*.cpp"
)

hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)

hunter_add_package(stb)
find_package(stb CONFIG REQUIRED)

hunter_add_package(Assimp)
find_package(Assimp CONFIG REQUIRED)

hunter_add_package(glm)
find_package(glm REQUIRED)

set(PROJECT_LIBS SDL2::SDL2main SDL2::SDL2 stb::stb
    Assimp::assimp glm)


if(WIN32)
    hunter_add_package(glew)
    find_package(glew CONFIG REQUIRED)

    set(PROJECT_LIBS ${PROJECT_LIBS} glew::glew)

    add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
else()
    #TODO: OPTICK_ENABLED=OFF
    add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
endif()

add_subdirectory(dep)
set(PROJECT_LIBS ${PROJECT_LIBS} OptickCore)

target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS})