
add_library(serial
    $<$<PLATFORM_ID:Linux>:serial_posix.cpp>
    $<$<PLATFORM_ID:Windows>:serial_win32.cpp>
)

target_include_directories(serial
    PUBLIC
        .
)
