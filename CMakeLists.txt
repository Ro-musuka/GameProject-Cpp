cmake_minimum_required(VERSION 4.4)

project(Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(
    GLM_ENABLE_EXPERIMENTAL
    TINYGLTF3_ENABLE_FS
)

find_package(SDL3 REQUIRED CONFIG)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    # gltf_loader用の処理
    # ${CMAKE_SOURCE_DIR}/src/external/tiny_gltf_v3.c
)

add_executable(main ${SOURCES})

target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(main PRIVATE
    SDL3::SDL3
    glm::glm
    OpenGL::GL
    GLEW::GLEW
    assimp::assimp
)