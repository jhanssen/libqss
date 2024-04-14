cmake_minimum_required(VERSION 3.10)

project(qss)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(Qt6 REQUIRED COMPONENTS Core)

add_subdirectory(3rdparty)

file(GLOB SRCS "src/*.cpp" "include/*.h")
add_library(${PROJECT_NAME} STATIC ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} ctre::ctre)

#add_executable(test_${PROJECT_NAME} test/test.cpp)
#target_link_libraries(test_${PROJECT_NAME} Qt::Core ${PROJECT_NAME})

