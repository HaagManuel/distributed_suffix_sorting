cmake_minimum_required(VERSION 3.13)
project(distributed_string_sorting)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_NINJA_PARALLEL_PROCESSING ON)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

# -Werror stops compiliation because of some files in KaDiS
list(APPEND
  DSSS_WARNING_FLAGS
  # "-Werror"
  "-Wall"
  "-Wextra"
  "-Wundef"
  "-Wunreachable-code"
  "-Wno-unused-parameter"
  # "-Wpedantic"
  "-Wnull-dereference"
  "-Wimplicit-fallthrough"
  "-Wno-vla"
  "-Wno-pessimizing-move"
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  list(APPEND DSSS_WARNING_FLAGS "-Wno-gnu-zero-variadic-macro-arguments")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -DDEBUG -march=native -ggdb")

# compile options
option(OPTIMIZE_DATA_TYPES "use smallest data type possible for ranks in pdcx" OFF)
option(INCLUDE_ALL_SORTERS "include all axtmann sorters" OFF)
message(STATUS "---> OPTIMIZE_DATA_TYPES: ${OPTIMIZE_DATA_TYPES}")
message(STATUS "---> INCLUDE_ALL_SORTERS: ${INCLUDE_ALL_SORTERS}")



# Default to Release builds
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UPPER)
message(STATUS "Build Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE_UPPER}}")

find_package(MPI REQUIRED)
message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")

set(TLX_USE_LTO ON)
add_subdirectory(external/tlx)

# IPS40 doesn't compile wihtout linking to TBB
set(IPS4O_DISABLE_PARALLEL OFF)
add_subdirectory(external/ips4o)

add_subdirectory(external/kamping)
add_subdirectory(external/KaDiS)


# Create a library containing all implementation, to be linked against by executables
add_library(dss_base)
set_target_properties(dss_base PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(dss_base PRIVATE ${DSSS_WARNING_FLAGS})

target_include_directories(dss_base PRIVATE
  "${CMAKE_CURRENT_BINARY_DIR}/include")

message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(dss_base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(dss_base PRIVATE kamping)
target_link_libraries(dss_base PRIVATE MPI::MPI_CXX)
target_link_libraries(dss_base PUBLIC tlx)
target_link_libraries(dss_base PUBLIC ips4o)
target_link_libraries(dss_base PRIVATE kadis)

set(StringSorting "src/string_sorting")
add_subdirectory(src)

# main
# add_executable(main src/main.cpp)
add_executable(main src/executables/main.cpp)
target_include_directories(main PRIVATE
  "${CMAKE_CURRENT_BINARY_DIR}/include")

target_compile_options(main PRIVATE ${DSSS_WARNING_FLAGS})
target_link_libraries(main kamping)
target_link_libraries(main dss_base)
target_link_libraries(main kadis)

# cli
add_executable(cli src/executables/cli.cpp)
target_include_directories(cli PRIVATE
  "${CMAKE_CURRENT_BINARY_DIR}/include")

target_compile_options(cli PRIVATE ${DSSS_WARNING_FLAGS})
target_link_libraries(cli kamping)
target_link_libraries(cli dss_base)
target_link_libraries(cli kadis)


# add compile flags
if(OPTIMIZE_DATA_TYPES)
  message(STATUS "setting OPTIMIZE_DATA_TYPES in main")
  message(STATUS "setting OPTIMIZE_DATA_TYPES in cli")
  target_compile_definitions(main PRIVATE OPTIMIZE_DATA_TYPES)
  target_compile_definitions(cli PRIVATE OPTIMIZE_DATA_TYPES)
endif()
  
if(INCLUDE_ALL_SORTERS)
  message(STATUS "setting INCLUDE_ALL_SORTERS in main")
  message(STATUS "setting INCLUDE_ALL_SORTERS in cli")
  target_compile_definitions(main PRIVATE INCLUDE_ALL_SORTERS)
  target_compile_definitions(cli PRIVATE INCLUDE_ALL_SORTERS)
endif()
