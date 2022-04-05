cmake_minimum_required (VERSION 3.23)

project(softrender)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SOURCES
	"${SRC_DIR}/${PROJECT_NAME}/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/*.cpp"
)

set(SDL2_DIR "C:/Libs/build_ready/SDL2-2.0.20-devel")
find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")

if(MSVC)
  target_compile_options(${PROJECT_NAME} PUBLIC "/GL")
endif()

