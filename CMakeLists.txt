cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
PROJECT(Test VERSION 1.0.0 LANGUAGES CXX)

OPTION(sanitize "Built with the sanitize support." OFF)
OPTION(coverage "Built with the gCov/lcov support." OFF)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})

# https://arne-mertz.de/2018/07/cmake-properties-options/
if (MSVC)
  # warning level 4 and all warnings as errors
  add_compile_options(/W4 /WX)
else()
  # lots of warnings and all warnings as errors
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

IF (sanitize)
  SET(CMAKE_BUILD_TYPE Debug)
  INCLUDE(sanitizers)
ENDIF()

IF (coverage)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

FIND_PACKAGE(Threads)

###############################################################################
# Title: Override option in CMake subproject
# Web: https://stackoverflow.com/questions/14061605/override-option-in-cmake-subproject
##############################################################################
OPTION(gtest_force_shared_crt "" ON)

###############################################################################
# Title: When should I use add_subdirectory and when ExternalProject
# Web: https://cmake.org/pipermail/cmake/2013-February/053429.html
#      https://cliutils.gitlab.io/modern-cmake/chapters/testing/googletest.html
###############################################################################
ADD_SUBDIRECTORY(3rd-party/googletest EXCLUDE_FROM_ALL)
INCLUDE_DIRECTORIES(3rd-party/googletest/googletest/include)
INCLUDE_DIRECTORIES(3rd-party/googletest/googlemock/include)

FILE(GLOB TEST_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
ADD_EXECUTABLE(MainTest ${TEST_SRC})
###############################################################################
# Title: Enabling C++11 And Later In CMake
# Web: https://crascit.com/2015/03/28/enabling-cxx11-in-cmake
###############################################################################
SET_TARGET_PROPERTIES(MainTest PROPERTIES 
  CXX_STANDARD 14
  CXX_STANDARD_PROPERTIES YES
  CXX_EXTENSIONS NO)

TARGET_LINK_LIBRARIES(MainTest PRIVATE gtest gtest_main)
TARGET_LINK_LIBRARIES(MainTest PRIVATE ${CMAKE_THREAD_LIBS_INIT})

# Code Coverage
IF (coverage)
  TARGET_COMPILE_OPTIONS(MainTest PRIVATE -O0 -Wall -fprofile-arcs -ftest-coverage)

  # Required for gCov
  SET_PROPERTY(TARGET MainTest APPEND_STRING PROPERTY LINK_FLAGS "-fprofile-arcs -ftest-coverage")

  # Configure Ctest
  ENABLE_TESTING()
  ADD_TEST(NAME MainTest COMMAND MainTest)

  # Code Coverage
  IF(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "^(Apple)?Clang$")
      INCLUDE(CodeCoverage)
      SETUP_TARGET_FOR_COVERAGE(${PROJECT_NAME}_coverage MainTest coverage)
  ENDIF()

ENDIF()

# most valuable code, following is attached with a blog
# https://github.com/kaizouman/gtest-cmake-example

# Solve the build issue on windows
# https://github.com/snikulov/google-test-examples

# This repo which includes the code coverage