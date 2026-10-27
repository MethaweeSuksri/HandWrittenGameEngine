cmake_minimum_required(VERSION 3.16)
project(Hand_written_game_engine 
    VERSION 0.0.1 
    LANGUAGES C CXX )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_subdirectory(vendored/glad/cmake)

# tell GLAD what to generate
glad_add_library(glad
    STATIC
    API gl:core=3.3
)

file( GLOB_RECURSE GAME_ENGINE_SOURCE
    "src/*.cpp"
    "src/*.h")

# Create your game executable target as usual
add_executable(Engine ${GAME_ENGINE_SOURCE} )

# Link to the actual SDL3 library and GLAD.
target_link_libraries(Engine PRIVATE
    SDL3::SDL3
    glad
)

add_custom_command(TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:Engine>/shaders"
)

add_custom_command(TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/images"
        "$<TARGET_FILE_DIR:Engine>/images"
)