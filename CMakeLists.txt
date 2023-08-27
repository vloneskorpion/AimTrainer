cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

#Compiler paths
set(CMAKE_CXX_COMPILER /usr/local/bin/g++-11)
set(CMAKE_C_COMPILER /usr/local/bin/gcc-11)
set(CMAKE_BUILD_TYPE Debug)

set(PROJECT_NAME AimTrainer)
project(${PROJECT_NAME}  VERSION 1.0)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
file(GLOB_RECURSE CPP_FILES "${SOURCE_DIR}/*.cpp")
add_executable(${PROJECT_NAME} main/main.cpp ${CPP_FILES})

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_library(ASSIMP_LIB libassimp.5.dylib libassimp.5.2.4.dylib libassimp.dylib PATHS /usr/local/lib)

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/dependencies)

target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
target_link_libraries(${PROJECT_NAME} "-framework IOKit")
target_link_libraries(${PROJECT_NAME} "-framework CoreVideo")
target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIB})

add_subdirectory(dependencies)

set(logl_root "${CMAKE_SOURCE_DIR}")
configure_file(config.h.in config.h)
include_directories(${CMAKE_BINARY_DIR})

# -- ALTERNATIVE -- #
#set(logl_root "${CMAKE_SOURCE_DIR}")
#add_definitions(-DLOGL_ROOT="${logl_root}")
