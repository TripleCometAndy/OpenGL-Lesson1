cmake_minimum_required(VERSION 3.20)

# set the project name
project(GLPlayground)

include(FetchContent)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FetchContent_Declare(
	glfw-stable
	GIT_REPOSITORY https://github.com/TripleCometAndy/glfw-stable
	GIT_TAG b43c122dd194d74996d76c574a46d4bc23d6c7b0
)

FetchContent_MakeAvailable(glfw-stable)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG efec5db081e3aad807d0731e172ac597f6a39447
)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(
	CollisionMap
	GIT_REPOSITORY https://github.com/TheGameCave/CollisionMap
	GIT_TAG 594edb80c5d2d4f5e615b4ec3fc10c6b6725e167
)

FetchContent_MakeAvailable(CollisionMap)

add_executable(GLPlayground)

target_include_directories(GLPlayground PRIVATE
    include
    src
)

target_link_libraries(GLPlayground glfw glm CollisionMap)

add_subdirectory(src)