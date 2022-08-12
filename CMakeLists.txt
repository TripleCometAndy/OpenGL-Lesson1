cmake_minimum_required(VERSION 3.20)

# set the project name
project(GLPlayground)

include(FetchContent)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#enable compiler warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(GLPlayground PRIVATE -Wall -Wextra)
endif()

FetchContent_Declare(
	glfw-stable
	GIT_REPOSITORY https://github.com/TripleCometAndy/glfw-stable
	GIT_TAG b43c122dd194d74996d76c574a46d4bc23d6c7b0
)

FetchContent_MakeAvailable(glfw-stable)

add_executable(GLPlayground)

target_include_directories(GLPlayground PRIVATE
    include
    src
)

target_link_libraries(GLPlayground glfw)

add_subdirectory(src)