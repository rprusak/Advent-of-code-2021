cmake_minimum_required(VERSION 3.10)

# set the project name
project(Starter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("${PROJECT_BINARY_DIR}" ../common)
add_compile_options(-Wall -Wextra -Werror)
add_executable(Starter main.cpp ../common/common.cpp)

set(GTEST_ROOT /usr/src/googletest/googletest)
find_package(GTest REQUIRED)

add_executable(Test test.cpp)
target_link_libraries(Test PUBLIC "${GTEST_LIBRARIES}" pthread)
target_include_directories(Test PUBLIC "${GTEST_INCLUDE_DIRS}")

add_executable(reddit main2.cpp)