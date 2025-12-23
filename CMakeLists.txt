cmake_minimum_required(VERSION 3.16)

project(Satellite_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(APP_DIR ${CMAKE_SOURCE_DIR}/app)

file(GLOB_RECURSE SRC_FILES
    ${SRC_DIR}/*.cpp
)

add_library(core STATIC ${SRC_FILES})
target_include_directories(core
    PUBLIC ${INCLUDE_DIR}
)

file(GLOB APP_FILES
    ${APP_DIR}/*.cpp
)

add_executable(app ${APP_FILES})
target_link_libraries(app
    PRIVATE core
)