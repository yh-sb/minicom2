target("terminal")
    set_kind("object")
    add_includedirs(".", {public = true})
    if is_plat("windows", "mingw") then
        add_files("terminal_win32.cpp")
    else
        add_files("terminal_posix.cpp")
    end
