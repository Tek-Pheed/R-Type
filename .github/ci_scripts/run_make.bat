cmake .
if %errorlevel% neq 0 (
    echo "::error title=Build Checker windows::CMake Configuration Failed"
    exit 1
)

cmake -B build -S .

if %errorlevel% neq 0 (
    echo "::error title=Build Checker windows::CMake Configuration Failed"
    exit 1
)

make -C build > temp.txt 2>&1

if %errorlevel% neq 0 (
    echo "::error title=Build Checker windows::CMake Configuration Failed"
    exit 1
)
