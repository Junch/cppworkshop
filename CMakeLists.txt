cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
PROJECT(Test VERSION 1.0.0 LANGUAGES CXX)

OPTION(sanitize "Built with the sanitize support." OFF)
OPTION(coverage "Built with the gCov/lcov support." OFF)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})

# https://arne-mertz.de/2018/07/cmake-properties-options/
if (MSVC)
  # warning level 4 and all warnings as errors
  add_compile_options(/W4 /WX)
  add_compile_options(/D_CRTDBG_MAP_ALLOC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4100 /wd4740 /wd4267") # glog introduce a warning
  # make the Release version create a PDB
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
  # # minimize the size or the resulting EXE
  SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
else()
  # lots of warnings and all warnings as errors
  # https://stackoverflow.com/questions/15053776/how-do-you-disable-the-unused-variable-warnings-coming-out-of-gcc-in-3rd-party-c
  # add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-unused-parameter)
  # temporarily disable -Werror to pass build in linux
  add_compile_options(-Wall -Wextra -pedantic -Wno-unused-parameter -Wno-sign-compare -Werror)
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

###############################################################################
# Title: 正确地使用ExternalProject_Add添加外部项目到本地cmake项目
# https://gist.github.com/taogashi/33ebf6b4285ba96419b0aea3d7f4d63e
# https://git.cs.colorado.edu/caldweba/grappa-pmem/blob/master/third-party/CMakeLists.txt
# https://github.com/dev-cafe/cmake-cookbook/blob/master/chapter-08/recipe-01/cxx-example/CMakeLists.txt
###############################################################################
INCLUDE(ExternalProject)

SET (install_dir ${CMAKE_BINARY_DIR}/3rd-party)
INCLUDE_DIRECTORIES(${install_dir}/include)
LINK_DIRECTORIES(${install_dir}/lib)

if(UNIX AND NOT APPLE)
  LINK_DIRECTORIES(${install_dir}/lib64)
endif()

IF (APPLE)
  # brew install util-linux
  LINK_DIRECTORIES(/usr/local/Cellar/util-linux/2.37/lib)
ENDIF()

ExternalProject_Add(third-party-gflags
  SOURCE_DIR        ${CMAKE_CURRENT_LIST_DIR}/3rd-party/gflags
  PREFIX            ${CMAKE_BINARY_DIR}/gflags
  INSTALL_DIR       ${install_dir}
  CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                    -DBUILD_SHARED_LIBS=OFF
)

ExternalProject_Add( third-party-glog
  SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/glog
  PREFIX            ${CMAKE_BINARY_DIR}/glog
  INSTALL_DIR       ${install_dir}
  CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                    -DBUILD_SHARED_LIBS=OFF
  DEPENDS           third-party-gflags
)

#FILE(GLOB TEST_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
SET(TEST_SRC ${PROJECT_SOURCE_DIR}/src/async.cpp
             ${PROJECT_SOURCE_DIR}/src/cpp_17.cpp
             ${PROJECT_SOURCE_DIR}/src/function_lambda.cpp
             ${PROJECT_SOURCE_DIR}/src/function_signature.cpp
             ${PROJECT_SOURCE_DIR}/src/gflags_demo.cpp
             ${PROJECT_SOURCE_DIR}/src/glog_demo.cpp
             ${PROJECT_SOURCE_DIR}/src/inline_thread_local.cpp
             ${PROJECT_SOURCE_DIR}/src/main.cpp
             ${PROJECT_SOURCE_DIR}/src/map_test.cpp
             ${PROJECT_SOURCE_DIR}/src/method_chaining.cpp
             ${PROJECT_SOURCE_DIR}/src/optional.cpp
             ${PROJECT_SOURCE_DIR}/src/packaged_task.cpp
             ${PROJECT_SOURCE_DIR}/src/promise.cpp
             ${PROJECT_SOURCE_DIR}/src/shared_ptr.cpp
             ${PROJECT_SOURCE_DIR}/src/string_utils.cpp
             ${PROJECT_SOURCE_DIR}/src/stringftime.cpp
             ${PROJECT_SOURCE_DIR}/src/thread_pool.cpp
             ${PROJECT_SOURCE_DIR}/src/valgrind.cpp
             ${PROJECT_SOURCE_DIR}/src/variadic_template.cpp
             ${PROJECT_SOURCE_DIR}/src/condition_variable.cpp
             ${PROJECT_SOURCE_DIR}/src/hash_test.cpp
             ${PROJECT_SOURCE_DIR}/src/mock_test.cpp
             ${PROJECT_SOURCE_DIR}/src/random.cpp
             ${PROJECT_SOURCE_DIR}/src/bind_test.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/treenode.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_1114_print_in_order.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_1114_print_in_order_2.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_1115_print_foolbar_alternately.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_1116_print_zero_even_odd.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_1117_building_h2o.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_654_maximum_binary_tree.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_914_x_of_a_kind_in_a_deck_of_cards.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_350_intersection_of_two_arrays_ii.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_134_gas_station.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_334_increasing_triplet_subsequence.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_239_sliding_window_maximum.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_218_the_skyline_problem.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_15_3sum.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_996.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_212.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_127.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_698.cpp
             ${PROJECT_SOURCE_DIR}/src/algorithm/leet_388.cpp
  )

if (MSVC)
  list(APPEND TEST_SRC ${PROJECT_SOURCE_DIR}/src/win32/StackWalker.cpp)
  list(APPEND TEST_SRC ${PROJECT_SOURCE_DIR}/src/win32/walker_test.cpp)
  list(APPEND TEST_SRC ${PROJECT_SOURCE_DIR}/src/win32/wim_test.cpp)
endif()

ADD_EXECUTABLE(MainTest ${TEST_SRC})
###############################################################################
# Title: Enabling C++11 And Later In CMake
# Web: https://crascit.com/2015/03/28/enabling-cxx11-in-cmake
###############################################################################
SET_TARGET_PROPERTIES(MainTest PROPERTIES 
  CXX_STANDARD 17
  CXX_STANDARD_PROPERTIES YES
  CXX_EXTENSIONS NO
)

TARGET_LINK_LIBRARIES(MainTest PRIVATE gtest gmock)
TARGET_LINK_LIBRARIES(MainTest PRIVATE ${CMAKE_THREAD_LIBS_INIT})

if (MSVC)
  #############################################################################
  # MSVC/MinGW中导入glog静态库的正确方式
  # https://blog.csdn.net/10km/article/details/74564011
  #############################################################################
  # Cmake: Specifiy config specific settings for multi-config cmake project
  # https://stackoverflow.com/questions/46581729/cmake-specifiy-config-specific-settings-for-multi-config-cmake-project
  # https://stackoverflow.com/questions/24460486/cmake-build-type-not-being-used-in-cmakelists-txt
  # https://github.com/Microsoft/vcpkg/issues/4606
  # https://gitlab.kitware.com/cmake/cmake/issues/17645
  #############################################################################
  TARGET_COMPILE_DEFINITIONS(MainTest PUBLIC -DGOOGLE_GLOG_DLL_DECL=)
  TARGET_LINK_LIBRARIES(MainTest PRIVATE shlwapi.lib)
  TARGET_LINK_LIBRARIES(MainTest PRIVATE $<IF:$<CONFIG:Debug>,gflags_static_debug.lib,gflags_static.lib>)
  TARGET_LINK_LIBRARIES(MainTest PRIVATE $<IF:$<CONFIG:Debug>,glogd.lib,glog.lib>)
  target_compile_definitions(MainTest PUBLIC UNICODE _UNICODE)
else()
  TARGET_LINK_LIBRARIES(MainTest PRIVATE debug glogd optimized glog)
  TARGET_LINK_LIBRARIES(MainTest PRIVATE debug gflags_debug optimized gflags)
  TARGET_LINK_LIBRARIES(MainTest PRIVATE uuid)
endif()

ADD_DEPENDENCIES(MainTest third-party-glog)

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
