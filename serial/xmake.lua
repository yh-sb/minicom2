target("serial")
    set_kind("object")
    add_packages("boost")
    add_includedirs(".", {public = true})
    add_files("serial_boost.cpp")
    
    if is_plat("windows", "mingw") then
        add_files("serial_win32.cpp")
    else
        add_files("serial_posix.cpp")
    end
