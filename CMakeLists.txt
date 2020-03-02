cmake_minimum_required(VERSION 3.15)
project(libreism)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.61.0 REQUIRED)

include_directories(
        include
        ${Boost_INCLUDE_DIRS}
)

set(SRC_LIST
        src/main.cpp
        src/time/Time.cpp)

add_subdirectory(drogon)

add_executable(${PROJECT_NAME}
        ${SRC_LIST})

target_link_libraries(${PROJECT_NAME}
        PRIVATE drogon)