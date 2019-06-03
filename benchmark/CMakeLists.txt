cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(BenchTest VERSION 1.0.0 LANGUAGES CXX)

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
include(ExternalProject)

set (install_dir ${CMAKE_BINARY_DIR}/3rd-party)
include_directories(${install_dir}/include)
link_directories(${install_dir}/lib)

ExternalProject_Add( third-party-benchmark
  SOURCE_DIR        ${CMAKE_CURRENT_LIST_DIR}/../3rd-party/benchmark
  PREFIX            ${CMAKE_BINARY_DIR}/benchmark
  INSTALL_DIR       ${install_dir}
  CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                    -DBENCHMARK_ENABLE_TESTING=OFF
)

file(GLOB_RECURSE ALL_BENCH_CPP *.cpp)
list(FILTER ALL_BENCH_CPP EXCLUDE REGEX "CMakeCXXCompilerId.cpp")

foreach(ONE_BENCH_CPP ${ALL_BENCH_CPP})
  get_filename_component(ONE_BENCH_EXEC ${ONE_BENCH_CPP} NAME_WE)

  # Avoid name collision
  set(TARGET_NAME Bench_${ONE_BENCH_EXEC})

  add_executable(${TARGET_NAME} ${ONE_BENCH_CPP})

  set_target_properties(${TARGET_NAME} PROPERTIES 
    CXX_STANDARD 17
    CXX_STANDARD_PROPERTIES YES
    CXX_EXTENSIONS NO
    OUTPUT_NAME ${ONE_BENCH_EXEC}
  )

  target_link_libraries(${TARGET_NAME} PRIVATE benchmark)

  if (MSVC)
    target_link_libraries(${TARGET_NAME} PRIVATE shlwapi.lib)
  endif()

  add_dependencies(${TARGET_NAME} third-party-benchmark)
endforeach()
