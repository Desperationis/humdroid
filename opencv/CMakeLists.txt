set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

cmake_minimum_required(VERSION 3.5)

find_package(OpenCV REQUIRED)

add_executable(GPU gpu.cpp) 
target_include_directories(GPU PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(GPU PUBLIC third-party/json/single_include)
target_include_directories(GPU PUBLIC ./)
target_link_libraries(GPU ${OpenCV_LIBS})


add_executable(CPUU cpu.cpp) 
target_include_directories(CPUU PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(CPUU PUBLIC third-party/json/single_include)
target_include_directories(CPUU PUBLIC opencv_wrapper)
target_link_libraries(CPUU ${OpenCV_LIBS})
