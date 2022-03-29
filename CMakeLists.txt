cmake_minimum_required(VERSION 3.21)
project(Tree)

set(CMAKE_CXX_STANDARD 14)

set(ConanFolder conanBuild)
include(${CMAKE_CURRENT_SOURCE_DIR}/${ConanFolder}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/modules/*.h) #collect all .h files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/modules/*.cpp) #collect all .cpp files

set(SOURCES ${HEADER_FILES} ${SOURCE_FILES})

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/modules) # need for include project files