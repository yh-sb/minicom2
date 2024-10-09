add_rules("mode.debug", "mode.release", "plugin.compile_commands.autoupdate", {outputdir = "build"})
set_defaultmode("debug")
set_targetdir("$(buildir)")
set_languages("c++20")

if is_plat("mingw") then
    add_ldflags("-static")
end

add_requires("boost", {configs = {asio = true, program_options = true}})

target("minicom2")
    add_packages("boost")
    includes("serial", "terminal")
    add_deps("serial", "terminal")
    add_files(
        "main.cpp",
        "cli_options.cpp"
    )
    
    if is_plat("mingw", "windows") then
        add_syslinks("ws2_32", "wsock32")
        add_files("icon.rc")
    end
