cmake_minimum_required(VERSION 3.16)
project(hello)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# SDL3
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# GLAD
add_subdirectory(vendored/glad/cmake)

# tell GLAD what to generate
glad_add_library(glad
    STATIC
    API gl:core=3.3
)

# Create your game executable target as usual
add_executable(hello WIN32 hello.cpp)

# Link to the actual SDL3 library and GLAD.
target_link_libraries(hello PRIVATE
    SDL3::SDL3
    glad
)