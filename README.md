# V4Hero

![Patafour logo](logo.png)

An official client for PatafourV4.

## Table of Contents

- [Setup](#setup)
  - [Required Libraries and Tools](#required-libraries-and-tools)
  - [Note for Linux Users](#note-for-linux-users)
  - [Note for Windows Users](#note-for-windows-users)
- [Contributing](#contributing)
- [License](#license)


## Setup

After cloning the repo, get a copy the `resources` folder from an existing Patafour install or request an updated copy from a developer on [Discord](https://discord.gg/TgWqp5WH) and copy the folder in the project root directory.

### Required Libraries and Tools

 * [CMake](https://cmake.org/download/) >= 3.19
 * [SFML](https://www.sfml-dev.org/download.php) >= 2.5

### Note for Linux users

On Linux also the X11 library needs to be installed (if you're on Ubuntu it's the `libx11-dev` package and you can install it with `sudo apt install libx11-dev`).

Note that currently we only support the proprietary NVidia driver for NVidia GPUs.

### Note for Windows users

You should enable [Developer Mode](https://docs.microsoft.com/en-us/windows/apps/get-started/enable-your-device-for-development) in order to successfully configure the project.

Please also make sure that the environment variable `SFML_ROOT` points to a valid SFML installation and that the SFML installation is also added to PATH.


## Contributing

PRs are welcome.

The developer team and the wider community around Patafour hangs out on [Discord](https://discord.gg/TgWqp5WH), 
and we use that to coordinate the efforts around development. 

Please contact the developer team on Discord if you intend to make wide changes to the game code, otherwise feel free to open issues or send PRs!


## License

GPL-3.0 (c) the Patafour team
