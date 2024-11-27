cmake .
if ($?) {
    cmake -B build -S .
} else {
    Write-Output "::error title=Build Checker windows::CMake Configuration Failed"
    exit 1
}

if ($?) {
    make -C build > temp.txt 2>&1
} else {
    Write-Output "::error title=Build Checker windows::CMake Configuration Failed"
    exit 1
}

if (-not $?) {
    Write-Output "::error title=Build Checker windows::CMake Configuration Failed"
    exit 1
}
