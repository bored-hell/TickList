cmake_minimum_required(VERSION 3.10)
project(TickList VERSION 1.0 LANGUAGES CXX)

# 1. Set C++ Standard and Flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

# 2. Handle Build Types (Debug vs Release)
# CMake handles -g and -O2 automatically based on CMAKE_BUILD_TYPE.
# If no build type is passed, default to Release.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose build type" FORCE)
endif()

# 3. Find wxWidgets (Replacing `wx-config`)
# 'core' and 'base' are the components requested in your script
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# 4. Define Sources
set(SRC
    main.cpp
    gui.cpp
    core.cpp
)

# 5. Define the Executable
add_executable(TickList ${SRC})

# 6. Link Libraries
target_link_libraries(TickList PRIVATE ${wxWidgets_LIBRARIES})

# 7. Set Output Directory (Replacing BUILD_FOLDER="bin")
set_target_properties(TickList PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
