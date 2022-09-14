set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

cmake_minimum_required(VERSION 3.5)

find_package(OpenCV REQUIRED)

add_executable(output main.cpp) 

target_include_directories(output PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(output ${OpenCV_LIBS})
