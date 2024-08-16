# V4Hero

![Patafour logo](logo.png)

An official client for Patafour.

## Table of Contents

- [Setup](#setup)
  - [Linux installation](#linux-installation)
  - [Windows installation](#windows-installation)
- [Contributing](#contributing)
- [License](#license)

### Required Libraries and Tools

 * [CMake](https://cmake.org/download/) >= 3.19
 * [Git](https://git-scm.com/downloads)
 * [SFML](https://www.sfml-dev.org/download.php) >= 2.5
 * [Discord Game SDK](https://dl-game-sdk.discordapp.net/2.5.6/discord_game_sdk.zip)
 
## Setup

Simply clone the repo and CMake should automatically grab everything for you, if your IDE supports it. 

Libraries may be needed, refer to the installation sections below.

### IDE

Visual Studio Community 2019+, Visual Studio Code and CLion generally should work just fine. 

Code::Blocks and others haven't been tested and probably won't work.

### Compiler

GCC is used to compile Patafour alongside with it's required libraries. C++20 support is required.

If you're running into issues with "fmt" and std::format, your GCC version is too old.

MSVCRT runtime should work. [UCRT runtime won't work.](https://en.sfml-dev.org/forums/index.php?topic=28265.0) We personally use MinGW on Windows.

## Linux installation

You need to have all the libraries necessary to compile SFML in order to compile Patafour. It all depends on the flavor of Linux you're using.

After installing the dependencies, cloning the repository and launching `cmake .` in the repo's folder should build everything properly. 

When the compilation is done, you can just run `./V4Hero` and the game will launch.

### Ubuntu / Debian

```sudo apt install build-essential libx11-dev libxrandr-dev libxcursor-dev libgl1-mesa-dev libudev-dev libopenal-dev libvorbis-dev libflac-dev libfreetype-dev libfreetype6 libfreetype6-dev zlib1g-dev libzip-dev liblzma-dev libbz2-dev```

### Fedora / Red Hat

```sudo dnf install make automake gcc gcc-c++ libstdc++-static kernel-devel SFML-devel systemd-devel libzip-devel libXcursor-devel libXrandr-devel mesa-libGL-devel```

### GPU support

Builds are known to work on NVIDIA systems without issues. AMD probably works as well, further tests are needed. Intel GPU support is unknown.

## Windows installation

The following instructions are for building Patafour on a 64-bit Windows system, using MinGW toolchain. Things may differ if you decide to use different tools, toolchains, compilers or architecture. 

Make sure you have [Git](https://git-scm.com/downloads) installed.

### Getting the libraries

* Install [MSYS](https://www.msys2.org/) preferably in `C:/msys64` directory. **We will need the MSYS installation directory in the next steps!**
* Make sure to always run **MSYS MINGW64**!!!!! Otherwise it will most likely not work! If you see **MINGW64** in your MSYS window, you're good to go.
* Run command `pacman -Syu`. Agree to everything (type the letter Y). The terminal might restart to update. Run **MSYS MINGW64** again if it does.
* Run command `pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-make mingw-w64-x86_64-libzip mingw-w64-x86_64-zlib`. Agree to installation (type the letter Y).
* Add the MSYS `bin` folder to your global **PATH** environmental variable. When selecting the install directory from the first step, it would be `C:/msys64/mingw64/bin`. [Here is how you can add a new folder to your PATH.](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/)
* If you had any other programs opened (such as IDE or Command Prompt), reopen them so they can detect the new environmental variable.

### Preparing the IDE

Our preferred IDE of choice is either Visual Studio Code or CLion. For Visual Studio Code, you want to install the "**C++ Extension Pack**" from the extensions store. It will help tremendously with integrating CMake and building the application.

### Post compilation
After everything is compiled, simply copy the `discord_game_sdk.dll` and `openal32.dll` from the `dlls` directory into the `build` directory (or wherever else V4Hero.exe is). 
If you did all the previous steps, everything should **just work** after running V4Hero.exe.

Potential mistakes and errors during Windows installation:

* Couldn't create `resources` symlink, permission error / Could not load tips background file!

CMake failed to make a symlink due to lack of permissions. Run the IDE as admin, or copy the `resources` folder where V4Hero.exe is.

* 0x8000007B error

Architecture mismatch. You compiled the project for x64 architecture (64-bit) and used DLLs for the x86 architecture (32-bit). Both project's and DLL's architectures need to match. If you don't know where the issue is, you can use a tool such as [Dependency Walker](https://www.dependencywalker.com/) to guide you what DLLs are wrong.

## Contributing

PRs are welcome.

The developer team and the wider community around Patafour hangs out on [Discord](https://discord.gg/dawfDyM), 
and we use that to coordinate the efforts around development. 

Please contact the developer team on Discord if you intend to make wide changes to the game code, otherwise feel free to open issues or send PRs!


## 3rd-party credits

  * [SFML](https://github.com/SFML/SFML) - multimedia library holding the project together
  * [nlohmann/json](https://github.com/nlohmann/json) - wonderful and simple json support
  * [gabime/spdlog](https://github.com/gabime/spdlog) - much better and more detailed logs
  * [ctabin/libzippp](https://github.com/ctabin/libzippp) - .zip file support


## License

GPL-3.0 (c) the Patafour team
