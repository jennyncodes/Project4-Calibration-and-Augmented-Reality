cmake_minimum_required(VERSION 3.10)
project(Project4_Camera_Calibration_AR)

# C++23 for modern features like std::println
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include the project headers (include/ is one level above build/)
include_directories(${CMAKE_SOURCE_DIR}/../include)

# Output all executables to bin/ instead of build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         ${CMAKE_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/../bin)

# Each executable only needs its own .cpp
add_executable(calibration calibration.cpp)
target_link_libraries(calibration ${OpenCV_LIBS})

add_executable(features features.cpp ${SHARED_SOURCES})
target_link_libraries(features ${OpenCV_LIBS})