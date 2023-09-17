cmake_minimum_required(VERSION 3.15)

project(minicom2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(serial)
add_subdirectory(terminal)

set(BOOST_INCLUDE_LIBRARIES asio program_options)
add_subdirectory(third_party/boost)

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    cli_options.cpp
    $<$<PLATFORM_ID:Windows>:${CMAKE_BINARY_DIR}/icon.rc>
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-static-libgcc;-static-libstdc++;$<$<PLATFORM_ID:Windows>:-static>>
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        serial
        terminal
        Boost::program_options
        $<$<PLATFORM_ID:Linux>:pthread>
)

# Deploy
if(WIN32)
    # Create resource file to set .exe icon
    file(WRITE ${CMAKE_BINARY_DIR}/icon.rc "IDI_ICON1 ICON DISCARDABLE \"${CMAKE_CURRENT_LIST_DIR}/icon.ico\"")
    install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION .)
else()
    # Create AppImage
    install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION "${CMAKE_BINARY_DIR}/appdir/usr/bin")
    install(CODE "execute_process(
        COMMAND linuxdeploy.AppImage --appdir appdir --desktop-file ${CMAKE_CURRENT_LIST_DIR}/app.desktop --icon-file ${CMAKE_CURRENT_LIST_DIR}/icon.png --output appimage
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND_ERROR_IS_FATAL ANY)"
    )
    # Get rid of hash and cpu arch in AppImage name
    install(CODE "execute_process(
        COMMAND sh -c \"mv ${CMAKE_PROJECT_NAME}*.AppImage ${CMAKE_PROJECT_NAME}.AppImage\"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND_ERROR_IS_FATAL ANY)"
    )
    install(FILES ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.AppImage DESTINATION .
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    )
endif()
