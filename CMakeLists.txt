cmake_minimum_required(VERSION 3.15)

if((APPLE))
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
endif((APPLE))


execute_process(
    COMMAND python -m pylmcf --include
    OUTPUT_VARIABLE PYLMCF_INCLUDE_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)


project(wasserstein_mcf_ext)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(nanobind REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG -g -O0")

nanobind_add_module(wasserstein_mcf_ext
SHARED NOMINSIZE
src/cpp/wasserstein_mcf/main.cpp)

target_include_directories(wasserstein_mcf_ext
PRIVATE
    ${PYLMCF_INCLUDE_PATH})


install(TARGETS wasserstein_mcf_ext LIBRARY DESTINATION wasserstein_mcf)
