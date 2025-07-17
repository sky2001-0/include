cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(CXXSupport VERSION 1.0 LANGUAGES CXX)

add_library(CXXSupport INTERFACE)

target_compile_features(CXXSupport INTERFACE cxx_std_20)

target_include_directories(CXXSupport INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>
)


enable_testing()

add_executable(test_cxx tests.cpp)
target_link_libraries(test_cxx PRIVATE CXXSupport)

add_test(NAME Vector3Test COMMAND test_cxx)
