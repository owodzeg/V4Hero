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
 * [Git](https://git-scm.com/downloads)
 * [SFML](https://www.sfml-dev.org/download.php) >= 2.5
 * [Discord Game SDK](https://dl-game-sdk.discordapp.net/2.5.6/discord_game_sdk.zip)
 
## Setup

Simply clone the repo and CMake should automatically grab everything for you. Visual Studio Code is strongly preferred and has been thoroughly used by the dev team, which implies best compatibility.

For other IDEs, Visual Studio Community 2019+ should work just fine. Code::Blocks and others haven't been tested and probably wont work.

### Compiler

GCC is used to compile Patafour alongside with it's required libraries.

### Linux installation

On Linux you also need to install the X11 library. On Ubuntu you can install it with `sudo apt install libx11-dev`.

Note that we currently only support the proprietary NVidia driver for NVidia GPUs.

### Windows installation

After building, an error may appear in relation to a missing OpenAL32.dll. You can simply copy it from SFML/bin folder to V4Hero.exe directory. The same thing applies to a missing discord_game_sdk.dll, which can be copied from the lib folder within Discord Game SDK package.

Make sure that you select proper architecture of the libraries. If you're experiencing a 0xC000007B error, that means the architecture of either OpenAL32.dll or discord_game_sdk.dll does not match the target architecture you're compiling V4Hero at.

[SFML has issues building on UCRT runtime.](https://en.sfml-dev.org/forums/index.php?topic=28265.0) Please make sure that your compiler uses MSVCRT runtime.


## Contributing

PRs are welcome.

The developer team and the wider community around Patafour hangs out on [Discord](https://discord.gg/dawfDyM), 
and we use that to coordinate the efforts around development. 

Please contact the developer team on Discord if you intend to make wide changes to the game code, otherwise feel free to open issues or send PRs!


## License

GPL-3.0 (c) the Patafour team
