
add_library(serial
    #$<$<PLATFORM_ID:Linux>:serial_posix.cpp>
    #$<$<PLATFORM_ID:Windows>:serial_win32.cpp>
    serial_boost.cpp
)

target_include_directories(serial
    PUBLIC
        .
)

target_link_libraries(serial
    PUBLIC
        Boost::asio
    PRIVATE
        $<$<PLATFORM_ID:Windows>:ws2_32>
)
