CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7 FATAL_ERROR)
PROJECT(Test)

# Required for gCov
OPTION(coverage "Built with the gCov/lcov support." OFF)

ADD_DEFINITIONS(
  -std=c++11
)

IF (coverage)
  SET(CMAKE_BUILD_TYPE Debug)

  ADD_DEFINITIONS(
    -g
    -O0
    -Wall
    -fprofile-arcs    # Required for gCov
    -ftest-coverage   # Required for gCov  
  )
ENDIF()

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})
GET_FILENAME_COMPONENT(DEPS_ROOT "${PROJECT_BINARY_DIR}/ThirdParty" ABSOLUTE)

FIND_PACKAGE(Threads)

INCLUDE(ExtProjectUtils)

IF(WIN32)
    set(_OPT_CMAKE_ARGS "-Dgtest_force_shared_crt=ON;-DCMAKE_SH=CMAKE_SH-NOTFOUND")
ELSE()
    set(_OPT_CMAKE_ARGS "")
ENDIF()

# Will download external CMakeable project from git repo, branch "master" and install it in $DEPS_ROOT
# This also will create "googletest.git" target, which we'll use as dependency for our test project
ExtProjectGit("https://github.com/google/googletest.git" "master" ${DEPS_ROOT} CMAKE_ARGS "${_OPT_CMAKE_ARGS}")

INCLUDE_DIRECTORIES("${DEPS_ROOT}/include")
LINK_DIRECTORIES("${DEPS_ROOT}/lib")
LINK_DIRECTORIES("${DEPS_ROOT}/lib64")

#set(CMAKE_DEBUG_POSTFIX d)

#SET_TARGET_PROPERTIES(mylib PROPERTIES DEBUG_POSTFIX "d")

FILE(GLOB TEST_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
ADD_EXECUTABLE(MainTest ${TEST_SRC})
ADD_DEPENDENCIES(MainTest "googletest.git")
TARGET_LINK_LIBRARIES(MainTest 
                      debug gtestd       optimized gtest
                      debug gtest_maind  optimized gtest_main)
TARGET_LINK_LIBRARIES(MainTest ${CMAKE_THREAD_LIBS_INIT})

# Code Coverage
IF (coverage)

  # Required for gCov
  SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

  # Configure Ctest
  ENABLE_TESTING()
  ADD_TEST(NAME MainTest COMMAND MainTest)

  # Code Coverage
  IF(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      INCLUDE(CodeCoverage)
      SETUP_TARGET_FOR_COVERAGE(${PROJECT_NAME}_coverage MainTest coverage)
  ENDIF()

ENDIF()

# most valuable code, following is attached with a blog
# https://github.com/kaizouman/gtest-cmake-example

# Solve the build issue on windows
# https://github.com/snikulov/google-test-examples

# This repo which includes the code coverage