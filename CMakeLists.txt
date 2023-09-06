cmake_minimum_required(VERSION 3.20)

project(CCLogger
  VERSION 0.1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

enable_testing()

add_executable(cc_logger
  ${CMAKE_SOURCE_DIR}/src/main.cc
  ${CMAKE_SOURCE_DIR}/src/logger.cc
)

target_include_directories(cc_logger PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)

add_subdirectory(test)
add_subdirectory(doc)
