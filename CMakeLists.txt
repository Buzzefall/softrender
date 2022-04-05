cmake_minimum_required (VERSION 3.23)

project(softrender)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SOURCES "${PROJECT_DIR}/${PROJECT_NAME}/*.cpp")

set(SDL2_DIR "${PROJECT_DIR}/libs/SDL2")
find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_DIR}")

if(MSVC)
  target_compile_options(${PROJECT_NAME} PUBLIC "/GL")
  target_link_options(${PROJECT_NAME} PUBLIC "/LTCG")
endif()

set(CMAKE_INSTALL_PREFIX "${PROJECT_DIR}/built_application")

install(TARGETS "${PROJECT_NAME}" DESTINATION .)
install(FILES "${SDL2_DIR}/lib/x64/SDL2.dll" DESTINATION .)
install(DIRECTORY "${PROJECT_DIR}/assets" DESTINATION .)
