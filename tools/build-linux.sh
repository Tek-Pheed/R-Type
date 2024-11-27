#!/bin/bash

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd $SCRIPTPATH

echo $SCRIPTPATH
printf "Installing Conan\n"


python3 -m venv conan-venv
source conan-venv/bin/activate
pip install conan
cd conan-venv/bin
export PATH="/Directory1:$PATH"

cd $SCRIPTPATH/../


if conan install -c tools.system.package_manager:mode=install . --output-folder=build --build=missing; then
    printf "Successfully installed required libraries\n"
else
    printf "Installation of libraries failed. Try to run this script with sudo privileges\n"
    exit 1
fi

printf "Building Project\n"
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake

cmake --build .