target("terminal")
    set_kind("object")
    add_includedirs(".", {public = true})
    if is_plat("windows") or is_plat("mingw") then
        add_files("terminal_win32.cpp")
    else
        add_files("terminal_posix.cpp")
    end
