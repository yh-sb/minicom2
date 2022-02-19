
add_library(terminal
    $<$<PLATFORM_ID:Linux>:terminal_posix.cpp>
    $<$<PLATFORM_ID:Windows>:terminal_win32.cpp>
)

target_include_directories(terminal
    PUBLIC
        .
)
