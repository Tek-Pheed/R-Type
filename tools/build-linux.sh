#!/bin/bash

function install_dependencies() {
    echo "### Checking system and installing dependencies"

    if [ -f /etc/debian_version ]; then
        echo "Detected Debian/Ubuntu-based distribution"
        PKG_MANAGER="apt"
        INSTALL_CMD="sudo apt install -y"
        DEPENDENCIES=(
            git gcc g++ make cmake
            libxrandr-dev libxcursor-dev libudev-dev libopenal-dev
            libflac-dev libvorbis-dev libgl1-mesa-dev libegl1-mesa-dev
            libdrm-dev libgbm-dev libcriterion-dev libfreetype-dev
        )

        for pkg in "${DEPENDENCIES[@]}"; do
            if ! dpkg -l | grep -q "^ii\s\+$pkg"; then
                echo "Installing missing dependency: $pkg"
                $INSTALL_CMD "$pkg"
            fi
        done
    elif [ -f /etc/redhat-release ]; then
        echo "Detected Fedora/RedHat-based distribution"
        PKG_MANAGER="dnf"
        INSTALL_CMD="sudo dnf install -y"
        DEPENDENCIES=(
            git gcc g++ make cmake
            freetype-devel libglvnd-opengl libXrandr-devel
            libXcursor-devel xrandr freetype glew libjpeg-turbo
            libsndfile openal-soft libvorbis-devel flac-devel
            libX11-devel libGL-devel systemd-devel openal-soft-devel
        )

        for pkg in "${DEPENDENCIES[@]}"; do
            if ! rpm -q "$pkg" &>/dev/null; then
                echo "Installing missing dependency: $pkg"
                $INSTALL_CMD "$pkg"
            fi
        done
    else
        echo "Unsupported operating system. Exiting."
        exit 1
    fi
}

install_dependencies

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
PROCS=$(nproc)

cd $SCRIPTPATH/..
printf "### Pulling dependent submodules\n"
git submodule update --init --recursive

printf "\n### Generating build files\n"
set -e
cmake -B build .
cd build
printf "\n### Building R-Type\n"
make -j $PROCS
printf "\n### Build done\n"

RELEASE_DIR=$SCRIPTPATH/../release/

printf "\n### Copying shared libraries (.so files)\n"
find . -type f \( -name "*.so" -o -name "*.so.*" \) -exec cp -P {} $RELEASE_DIR \;
find . -type f \( -name "*.dylib" -o -name "*.dylib" \) -exec cp -P {} $RELEASE_DIR \;

find . -type l \( -name "*.so" -o -name "*.so.*" \) -exec cp -P {} $RELEASE_DIR \;
find . -type l \( -name "*.dylib" -o -name "*.dylib" \) -exec cp -P {} $RELEASE_DIR \;

printf "\n### Shared libraries copied to release folder\n"
