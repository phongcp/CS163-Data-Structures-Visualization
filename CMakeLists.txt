cmake_minimum_required(VERSION 3.10)
project(DataStructureVisualization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include(FetchContent)  # Include module FetchContent to download raylib
# FetchContent_Declare(raylib
#     GIT_REPOSITORY https://github.com/raysan5/raylib.git
#     GIT_TAG 5.0
# )
# FetchContent_MakeAvailable(raylib)

set(RAYLIB_PATH "D:/raylib")

include_directories(${RAYLIB_PATH}/include)
link_directories(${RAYLIB_PATH}/lib)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(ds_visualization ${SOURCES}
                                      "src/tinyfiledialogs.c")

# if(WIN32)
#     add_executable(ds_visualization WIN32 ${SOURCES})
# else()
#     add_executable(ds_visualization ${SOURCES})
# endif()

target_link_libraries(ds_visualization raylib -lopengl32 -lgdi32 -lwinmm)
