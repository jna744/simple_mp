cmake_minimum_required(VERSION 3.13)

project(simple_meta_programming VERSION 0.1)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(SMP_PRIMARY_PROJECT true)
  set(CMAKE_EXPORT_COMPILE_COMMANDS true)
endif()

add_library(smp INTERFACE)
target_include_directories(smp INTERFACE include)

if(BUILD_TESTING AND SMP_PRIMARY_PROJECT)
  add_subdirectory(test)
endif()
