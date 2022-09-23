set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(OpenCV REQUIRED)

if(OpenCV_CUDA_VERSION)
	add_executable(humdroid_gpu gpu.cpp) 
	target_include_directories(humdroid_gpu PUBLIC ${OpenCV_INCLUDE_DIRS})
	target_include_directories(humdroid_gpu PUBLIC third-party/json/single_include)
	target_include_directories(humdroid_gpu PUBLIC ./)
	target_include_directories(humdroid_gpu PUBLIC util/)
	target_link_libraries(humdroid_gpu ${OpenCV_LIBS})
	install (TARGETS humdroid_gpu DESTINATION bin)
endif()


add_executable(humdroid_cpu cpu.cpp) 
target_include_directories(humdroid_cpu PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(humdroid_cpu PUBLIC third-party/json/single_include)
target_include_directories(humdroid_cpu PUBLIC ./)
target_include_directories(humdroid_cpu PUBLIC util/)
target_link_libraries(humdroid_cpu ${OpenCV_LIBS})
install (TARGETS humdroid_cpu DESTINATION bin)
