cmake_minimum_required(VERSION 3.0.2)
cmake_policy(VERSION 3.0.2)
cmake_policy(SET CMP0015 NEW)
option(EMSDK_LOC "Location of EMSDK" "C:/local/emsdk")

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_BUILD_DIRECTORY "/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build.emscripten)
#set(CMAKE_TOOLCHAIN_FILE ${EMSDK_LOC}"/emscripten/incoming/cmake/Modules/Platform/Emscripten.cmake")

project(wasmnoise)

option(WASMNOISE_LIB_MAJOR "Major Version Part" 0)
option(WASMNOISE_LIB_MINOR "Minor Version Part" 0)
option(WASMNOISE_LIB_PATCH "Patch Version Part" 0)
option(WASMNOISE_LIB_BUILD "Build Version Part" 0)
set(WASMNOISE_NAME wasmnoise-${WASMNOISE_LIB_MAJOR}.${WASMNOISE_LIB_MINOR}.${WASMNOISE_LIB_PATCH})
set(WASMNOISE_VERSION ${WASMNOISE_LIB_MAJOR}.${WASMNOISE_LIB_MINOR}.${WASMNOISE_LIB_PATCH}.${WASMNOISE_LIB_BUILD})


include_directories("include" ${EMSDK_LOC}"/emscripten/incoming/system/include")
file(GLOB_RECURSE CORE_HDR include/*.hpp)
file(GLOB_RECURSE CORE_SRC source/*.cpp)

add_definitions("-std=c++14 -v")
add_executable(${WASMNOISE_NAME} ${CORE_SRC} ${CORE_HDR})
#set_target_properties(${WASMNOISE_NAME} PROPERTIES LINK_FLAGS "-O3 -s WASM=1 -s BINARYEN_METHOD='native-wasm' -s BINARYEN_ASYNC_COMPILATION=1 -s NO_EXIT_RUNTIME=1  --bind")
#set_target_properties(${WASMNOISE_NAME} PROPERTIES LINK_FLAGS "-O0 -s BINARYEN_METHOD='native-wasm' -s WASM=1 -s ASSERTIONS=1 --bind")
set_target_properties(${WASMNOISE_NAME} PROPERTIES LINK_FLAGS "-O1 -s WASM=1 -s BINARYEN_ASYNC_COMPILATION=1 -s NO_EXIT_RUNTIME=1")