cmake_minimum_required(VERSION 3.13)

project(minicom2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(serial)
add_subdirectory(terminal)
add_subdirectory(third_party/argh)

if(MINGW)
    # https://github.com/boostorg/fiber/pull/281
    execute_process(COMMAND git -C third_party/boost/libs/fiber apply ../../../../mingw-fix-boost-fiber-build.patch ERROR_QUIET)
endif()
add_subdirectory(third_party/boost)

add_executable(${CMAKE_PROJECT_NAME} main.cpp cli_options.cpp)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-static-libgcc>
    $<$<CXX_COMPILER_ID:GNU>:-static-libstdc++>
    $<$<CXX_COMPILER_ID:GNU>:$<$<PLATFORM_ID:Windows>:-static>>
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        argh
        serial
        terminal
        $<$<PLATFORM_ID:Linux>:pthread>
)
