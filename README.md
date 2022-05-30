# V4Hero

![Patafour logo](logo.png)

An official client for Patafour.

## Table of Contents

- [Setup](#setup)
  - [Required Libraries and Tools](#required-libraries-and-tools)
  - [Note for Linux Users](#note-for-linux-users)
  - [Note for Windows Users](#note-for-windows-users)
- [Contributing](#contributing)
- [License](#license)

### Required Libraries and Tools

 * [CMake](https://cmake.org/download/) >= 3.19
 * [SFML](https://www.sfml-dev.org/download.php) >= 2.5
 * [Git](https://git-scm.com/downloads)
 
## Setup

Simply clone the repo and CMake should automatically grab everything for you. Visual Studio 2019+ is recommended.

### Linux installation

On Linux also the X11 library needs to be installed (if you're on Ubuntu it's the `libx11-dev` package and you can install it with `sudo apt install libx11-dev`).

Note that currently we only support the proprietary NVidia driver for NVidia GPUs.

### Windows installation

You should enable [Developer Mode](https://docs.microsoft.com/en-us/windows/apps/get-started/enable-your-device-for-development) in order to successfully configure the project.

Create an environment variable called `SFML_ROOT` that points to a valid SFML root directory.

After building, an error may appear that you're missing OpenAL32.dll. You can simply copy it from SFML/bin folder to V4Hero.exe directory.

Make sure that you select proper architecture when building. Downloading 32-bit SFML will only compile x86 builds and 64-bit SFML will only compile x64 builds.


## Contributing

PRs are welcome.

The developer team and the wider community around Patafour hangs out on [Discord](https://discord.gg/dawfDyM), 
and we use that to coordinate the efforts around development. 

Please contact the developer team on Discord if you intend to make wide changes to the game code, otherwise feel free to open issues or send PRs!


## License

GPL-3.0 (c) the Patafour team
