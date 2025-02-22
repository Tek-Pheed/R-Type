# Requirements

Our project uses external libraries included as submodules which are automatically built, however some submodules requires extra system-dependant libraries that you will need to install before building.

## Building on Linux

### Tools

You will need to have the following tools installed for building the project:

-   git
-   gcc
-   g++
-   make
-   cmake
-   Standard C and C++ libraries (probably already installed)

On Fedora/RedHat based distributions:

```
dnf install git gcc g++ make cmake
```

On Debian/Ubuntu based distributions:

```
apt install git gcc g++ make cmake
```

### Extra Dependencies
More information about required system libraries (for SFML): https://www.sfml-dev.org/tutorials/2.6/start-cmake.php

**On Fedora/RedHat based distributions:**


```
dnf install freetype-devel libglvnd-opengl libXrandr-devel libXcursor-devel xrandr freetype glew libjpeg-turbo libsndfile openal-soft libvorbis-devel flac-devel libX11-devel libGL-devel systemd-devel openal-soft-devel
```
>[!NOTE]
>
>_On Fedora you will have to install Criterion manually, since there is no package available. [This helper script worked for us](https://gist.github.com/Thibb1/065786184253d8e553efe7122ee82e82)_


**On Debian/Ubuntu based distributions:**

```
apt install libxrandr-dev libxcursor-dev libudev-dev libopenal-dev libflac-dev libvorbis-dev libgl1-mesa-dev libegl1-mesa-dev libdrm-dev libgbm-dev libcriterion-dev libfreetype-dev libfreetype6 libfreetype6-dev
```

### Building

Just run the script located in `tools/build-linux.sh`

The script will automatically pull all submodules and build the application.

## Building on Windows

### Tools

You will need to have the following tools installed for building the project:

-   [MSVC](https://visualstudio.microsoft.com/fr/downloads/) (Visual Studio >=2022, C++ app developpement pack)
-   [cmake](https://cmake.org/download/) (It might be automatically installed by Visual Studio)
-   [git](https://git-scm.com/downloads/win)

### Extra Dependencies

So far no extra dependencies are required on Windows.

### Building

Make sure unisgned powershell script execution is turned on.
If this is not the case, just run the command in an elevated powershell: `set-executionpolicy remotesigned`

Then, just run the script located in `tools/build-windows.ps1`

The script will automatically pull all submodules and build the application using your MSVC.


# Packaging and distribution

## Linux

**Easily share the project with friends on different Linux distributions**

We've made it simple to distribute the project to your friends, even if they use different Linux distributions, without them needing to recompile everything.

We provide a script, `tools/distribute-linux.sh`, that automates the build process and generates three packages in the `release/packages` folder:

- **A `.tar.gz` archive:** This archive contains the entire release folder, including dependencies, the server, the client, and all assets.
- **A Client AppImage:** This is a self-contained Linux executable for the game client. AppImages are convenient because they can run on most Linux distributions that support them, without requiring installation.
- **A Server AppImage:** This is a self-contained Linux executable for the game server, similar to the client AppImage.

By distributing these AppImages, you can share the project with your friends on almost any Linux distribution that supports AppImages.

>[!NOTE]
>
>The libfuse2 package is required to build appimages.

## Windows

### TODO: ZIP file