# Libreism

> Open Source Server Time Viewer written in **C++**

[![GitHub Release](https://img.shields.io/github/v/release/organization/Libreism?logo=github&logoColor=white&style=for-the-badge)](https://github.com/organization/Libreism/releases/)
![C++](https://img.shields.io/badge/c++-17-00599C?logo=c%2B%2B&logoColor=white&style=for-the-badge)
[![CMake](https://img.shields.io/badge/cmake->=3.15-064F8C?logo=cmake&logoColor=white&style=for-the-badge)](https://cmake.org/)
[![GitHub Workflow](https://img.shields.io/github/workflow/status/organization/Libreism/C++?logo=github&logoColor=white&style=for-the-badge)](https://github.com/organization/Libreism/actions)
[![License](https://img.shields.io/github/license/organization/Libreism?style=for-the-badge)](./LICENSE)

## Requirement

- C++ 17
- CMake(>=3.15)
- [Boost](https://www.boost.org/)(>=1.66)
- [Drogon](https://github.com/an-tao/drogon)(Git Submodule)
  - [JsonCpp](https://github.com/open-source-parsers/jsoncpp)
  - libuuid
  - OpenSSL
  - [TRANTOR](https://github.com/an-tao/trantor)(Git Submodule)
  - zlib

More information about Drogon Installation, see [Drogon Wiki](https://github.com/an-tao/drogon/wiki/02-Installation).

## Build

1. Clone the repository:

    ```bash
    $ git clone --recurse-submodules https://github.com/organization/Libreism.git
    ```

2. Build the source:

    ```bash
    $ cd Libreism
    $ cmake -S . -B build
    $ cmake --build build
    ```

    See [How to build a project](https://cliutils.gitlab.io/modern-cmake/chapters/intro/running.html)

## License

```text
Copyright 2020 Seungjae Park, Chansol Park, and JellyBrick

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

Libreism is licensed under the [Apache License 2.0](./LICENSE).
