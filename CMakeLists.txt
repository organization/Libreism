cmake_minimum_required(VERSION 3.15)

project(libreism
        VERSION 0.1.0
        DESCRIPTION "Open Source Server Time Viewer written in C++"
        HOMEPAGE_URL https://github.com/organization/Libreism
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Boost headers only
find_package(Boost 1.66 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Libreism
include_directories(include)
add_subdirectory(src)

# Drogon
add_subdirectory(drogon)
target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
