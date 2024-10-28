cmake_minimum_required(VERSION 3.10)
project(ServerLink)

# Set the output directory for the build files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set the compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
set(DEBUG_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# Add the source files
set(SOURCES
    main.cpp
    sl-ext.cpp
)

# Define the library and executable
add_library(sl-ext SHARED sl-ext.cpp)
add_executable(sl main.cpp)

# Link the shared library to the executable
target_link_libraries(sl sl-ext)

# Add custom commands for permissions and check targets

add_custom_target(check
    COMMAND ls
    COMMAND ${CMAKE_COMMAND} -E echo "Done!"
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sl.exe quit
    COMMENT "Running check target"
)

# Clean up target
add_custom_target(cleanup
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sl
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libsl-ext.so
    COMMENT "Cleaning up build files"
)
