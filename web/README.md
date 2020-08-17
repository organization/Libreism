# Libreism Web

> Open Source Server Time Viewer Web Page

![Node.js](https://img.shields.io/badge/node.js-14.0.0-339933?logo=node.js&logoColor=white&style=for-the-badge)
![Vue.js](https://img.shields.io/badge/vue.js-2.0-4FC08D?logo=vue.js&logoColor=white&style=for-the-badge)
[![GitHub Workflow](https://img.shields.io/github/workflow/status/organization/Libreism/Node.js?logo=github&logoColor=white&style=for-the-badge)](https://github.com/organization/Libreism/actions)
[![License](https://img.shields.io/github/license/organization/Libreism?style=for-the-badge)](./LICENSE)

## Build

### Requirement

- [Node.js](https://nodejs.org/)(>=14)
- [NPM](https://www.npmjs.com/)
  - See [`package.json`'s `dependencies` and `devDependencies`](./package.json)

### Guide

1. Clone the repository and go to `web` directory:

    ```bash
    $ git clone --recurse-submodules https://github.com/organization/Libreism.git
    $ cd Libreism/web
    ```

2. Install dependencies:

    ```bash
    $ npm ci
    ```

3. Build the static page:

    ```bash
    npm run build
    ```

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

Libreism Web is licensed under the [Apache License 2.0](./LICENSE).
