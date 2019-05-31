cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
PROJECT(BenchTest VERSION 1.0.0 LANGUAGES CXX)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})

# https://arne-mertz.de/2018/07/cmake-properties-options/
if (MSVC)
  # warning level 4 and all warnings as errors
  add_compile_options(/W4 /WX)
  add_compile_options(/D_CRTDBG_MAP_ALLOC)
else()
  # lots of warnings and all warnings as errors
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# https://felixmoessbauer.com/blog-reader/include-google-benchmark-test-in-c-project.html
# https://pixorblog.wordpress.com/2016/05/22/cmake-google-micro-benchmarking/
INCLUDE(ExternalProject)

SET (install_dir ${CMAKE_BINARY_DIR}/3rd-party)
INCLUDE_DIRECTORIES(${install_dir}/include)
LINK_DIRECTORIES(${install_dir}/lib)

ExternalProject_Add( third-party-benchmark
  SOURCE_DIR        ${CMAKE_CURRENT_LIST_DIR}/../3rd-party/benchmark
  PREFIX            ${CMAKE_BINARY_DIR}/benchmark
  INSTALL_DIR       ${install_dir}
  CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                    -DBENCHMARK_ENABLE_TESTING=OFF
)

FILE(GLOB TEST_SRC ${PROJECT_SOURCE_DIR}/*.cpp)
ADD_EXECUTABLE(BenchTest ${TEST_SRC})
###############################################################################
# Title: Enabling C++11 And Later In CMake
# Web: https://crascit.com/2015/03/28/enabling-cxx11-in-cmake
###############################################################################
SET_TARGET_PROPERTIES(BenchTest PROPERTIES 
  CXX_STANDARD 17
  CXX_STANDARD_PROPERTIES YES
  CXX_EXTENSIONS NO
)

TARGET_LINK_LIBRARIES(BenchTest PRIVATE debug benchmarkd optimized benchmark)
ADD_DEPENDENCIES(BenchTest third-party-benchmark)
