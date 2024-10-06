$ErrorActionPreference = "Stop"

$srcs = git ls-files '*.c' '*.cc' '*.cpp' '*.cxx' '*.c++' '*.h' '*.hpp' '*.hh'

Write-Host "clang-tidy -p build/compile_commands.json $srcs" -ForegroundColor Green
& 'clang-tidy' -p build/compile_commands.json $srcs
