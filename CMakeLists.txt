cmake_minimum_required(VERSION 3.13)

project(simple_meta_programming VERSION 0.1)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(SMP_PRIMARY_PROJECT true)
  set(CMAKE_EXPORT_COMPILE_COMMANDS true)
endif()

add_library(smp INTERFACE)

target_include_directories(
  smp INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

if(BUILD_TESTING AND SMP_PRIMARY_PROJECT)
  add_subdirectory(test)
endif()

install(
  TARGETS smp
  EXPORT smpTargets
  PUBLIC_HEADER DESTINATION include
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  smpConfigVersion.cmake
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(
  EXPORT smpTargets
  FILE smpConfig.cmake
  NAMESPACE smp::
  DESTINATION lib/cmake/smp
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
