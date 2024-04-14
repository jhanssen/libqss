cmake_minimum_required(VERSION 3.10)

project(qss)

add_subdirectory(3rdparty)

file(GLOB SRCS "src/*.cpp" "include/*.h")
add_library(${PROJECT_NAME} STATIC ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} ctre::ctre)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

#add_executable(test_${PROJECT_NAME} test/test.cpp)
#target_link_libraries(test_${PROJECT_NAME} Qt::Core ${PROJECT_NAME})

