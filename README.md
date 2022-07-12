# V4Hero

![Patafour logo](logo.png)

An official client for Patafour.

## Table of Contents

- [Setup](#setup)
  - [Required Libraries and Tools](#required-libraries-and-tools)
  - [Linux installation](#linux-installation)
  - [Windows installation](#windows-installation)
- [Contributing](#contributing)
- [License](#license)

### Required Libraries and Tools

 * [CMake](https://cmake.org/download/) >= 3.19
 * [SFML](https://www.sfml-dev.org/download.php) >= 2.5
 * [Git](https://git-scm.com/downloads)
 * [Discord Game SDK](https://dl-game-sdk.discordapp.net/2.5.6/discord_game_sdk.zip)
 
## Setup

Simply clone the repo and CMake should automatically grab everything for you. Visual Studio 2019+ is recommended.

### Linux installation

On Linux you also need to install the X11 library. On Ubuntu you can install it with `sudo apt install libx11-dev`.

Note that we currently only support the proprietary NVidia driver for NVidia GPUs.

### Windows installation

You should enable [Developer Mode](https://docs.microsoft.com/en-us/windows/apps/get-started/enable-your-device-for-development) in order to successfully configure the project.

Create an environment variable called `SFML_ROOT` that points to a valid SFML root directory.

After building, an error may appear in relation to a missing OpenAL32.dll. You can simply copy it from SFML/bin folder to V4Hero.exe directory. The same thing applies to a missing discord_game_sdk.dll, which can be copied from the lib folder within Discord Game SDK package.

Make sure that you select proper architecture when building. Downloading 32-bit SFML will only compile x86 builds and 64-bit SFML will only compile x64 builds.


## Contributing

PRs are welcome.

The developer team and the wider community around Patafour hangs out on [Discord](https://discord.gg/dawfDyM), 
and we use that to coordinate the efforts around development. 

Please contact the developer team on Discord if you intend to make wide changes to the game code, otherwise feel free to open issues or send PRs!


## License

GPL-3.0 (c) the Patafour team
