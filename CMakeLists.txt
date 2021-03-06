cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(signal_finder)

#include_directories( ../../include )

# set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# set the build type
set(CMAKE_BUILD_TYPE Release)

# set kernel source files
set(sf_kernel_src ./src/SignalFinder.cpp )
set(sf_kernel_src ${sf_kernel_src} ./src/SignalFinder2019.cpp )
set(sf_kernel_src ${sf_kernel_src} ./src/Buffers.cpp )
set(sf_kernel_src ${sf_kernel_src} ./src/ToolFunc.cpp )

# confirmation messages
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

find_package(OpenCV REQUIRED)

# signal_test
add_executable(signal_test ./src/TestMain.cpp ${sf_kernel_src})
add_definitions("-std=c++11")
target_link_libraries(signal_test ${OpenCV_LIBRARIES})

# find-signal
add_executable(find-signal ./src/find-signal.cpp ${sf_kernel_src})
target_link_libraries(find-signal ssm ypspur /usr/local/lib/kaw_lib.a ${OpenCV_LIBRARIES})

# test-command
add_executable(test-command ./src/test-command.cpp)
target_link_libraries(test-command ssm /usr/local/lib/kaw_lib.a ) 


