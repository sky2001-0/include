cmake_minimum_required(VERSION 3.0)

set(LIB_NAME CXXSupport)

project(${LIB_NAME})

add_library(${LIB_NAME} INTERFACE)

target_include_directories(${LIB_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)
