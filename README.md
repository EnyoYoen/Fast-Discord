# Fast-Discord
![Fast-Discord image](doc/images/Fast-Discord.png)
*For now it is very basic*

 - [Origins of this project](#origins)
 - [Coding conventions](#coding-conventions)
 - [License](#license)
 - [Build and Run](#build-and-run)
 - [Contact](#contact)

This project is really huge and requires a lot of time so any help is welcome, from the smallest syntax correction to major changes.
## Origins
The two main reasons that pushed me to create this project are: portability and client performance.
One day I wanted to install Discord on a Raspberry Pi, but I was running Ubuntu with an ARM processor, and Discord doesn't provide a version for computers running Linux and ARM processors.
And Discord is an application created with the Electron.js framework, so it uses a lot of resources, which is a shame for an application that you use in parallel with other applications most of the time.
## Coding Conventions
If you want to contribute to this project, you must respect the coding conventions from Qt (https://wiki.qt.io/Coding_Conventions).
## License
[MIT License](https://en.wikipedia.org/wiki/MIT_License)

Copyright (c) 2021 EnyoYoen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
## Build and Run
### Dependencies
- Qt5 (Widgets)
- Boost (filesystem)
- Libcurl
- OpenSSL (Crypto)
- Pthread
- Cpprestsdk
- Zlib

### Install the dependencies
**On Debian / Ubuntu :**
```shell
$ sudo apt-get install qt5-default libboost-filesystem-dev zlib1g libcurl4-openssl-dev libssl-dev libcpprest-dev libpthread-stubs0-dev
```

**On Windows (with [vcpkg](https://github.com/microsoft/vcpkg/#quick-start-windows)) :**
```shell
> vcpkg install qt5 boost-filesystem zlib curl openssl cpprestsdk pthreads
```

**On MacOS (with [Homebrew](https://brew.sh/index)) :**
```shell
$ brew install qt5 boost zlib curl openssl cpprestsdk libpthread-stubs
```

You will also need [CMake](https://cmake.org/download/) (Minimum version : 3.0)
### Build
In 'Fast-Discord/bin' folder :
```shell
cmake ..
cmake --build .
```
### Run
The executable is in the 'Fast-Discord/bin' folder ;)
## Contact
Fast-Discord's discord server : https://discord.gg/DezBvjrGve
