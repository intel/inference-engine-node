DISCONTINUATION OF PROJECT. 

This project will no longer be maintained by Intel.

This project has been identified as having known security escapes.

Intel has ceased development and contributions including, but not limited to, maintenance, bug fixes, new releases, or updates, to this project.  

Intel no longer accepts patches to this project.
# Inference Engine Binding for Node.js*

*Other names and brands may be claimed as the property of others.

## Prerequisites

Instal [Node.js](https://nodejs.org/).

Download [OpenVINO](https://software.intel.com/en-us/openvino-toolkit/choose-download/) and install it into the default path.

For Windows, install [Visual Studio 2019](https://visualstudio.microsoft.com/vs/).

For Linux, install `build-essential` package.

**Verified configurations:**
  * Node.js 12 LTS
  * OpenVINO 2021.4
  * Windows 10
  * Ubuntu Linux 18.04

## Install

The Inference Engine Binding for Node.js supports installation for two operation systems: Windows 10 and Ubuntu 16.04 and
two build systems: node-gyp and CMake-based and Node-GYP-based. 

### Install on Ubuntu 18.04

To install the Inference Engine Binding for Node.js on Ubuntu 16.04 use the following instruction:
1. Open a terminal in the repository root folder
2. Activate the OpenVINO environment:

If you installed the OpenVINO to the `/opt/intel/openvino` directory (as root) use the following command:

```shell script
$ source /opt/intel/openvino/bin/setupvars.sh
```

If you installed the OpenVINO to the home directory `~/intel/openvino` directory use the following command:

```shell script
$ source ~/intel/openvino/bin/setupvars.sh
```

To install Inference Engine Binding for Node.js using node-gyp use the following command in the same terminal:

```shell script
$ npm install
```

To install Inference Engine Binding for Node.js using cmake use following commands in the same terminal:

1. Set an environment variable `NODE_PATH` to directory with installed NodeJS. For example:
```shell script
$ export NODE_PATH=/home/user/.nvm/versions/node/v12.17.0/
```

2. Create an empty directory to build and go to this directory:
```shell script
$ mkdir "cmake-build" && cd "cmake-build"
```

3. Run cmake to fetch project dependencies and create Unix makefiles, then run make to build the project:
```shell script
$ cmake -DCMAKE_BUILD_TYPE=Release ../ && \
$ cmake --build . --target inference_engine_node -- -j $(nproc --all)
```

### Install on Windows 10

To install the Inference Engine Binding for Node.js on Windows 10 use the following instruction:
1. Open a terminal in the repository root folder
2. Activate the OpenVINO environment:
```shell script
> "C:\Program Files (x86)\IntelSWTools\openvino\bin\setupvars.bat"
```

To install Inference Engine Binding for Node.js using node-gyp use the following command in the same terminal:
```shell script
> npm install
```

**Note:** For "Error: MSBuild is not set" on VS 2019, please set `msbuild_path`, e.g.
```
> npm config set msbuild_path "c:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe"
```

To install Inference Engine Binding for Node.js using cmake use following commands in the same terminal:
1. Set an environment variable `NODE_PATH` to directory with installed node-gyp. For example:

```shell script
> set NODE_PATH=C:\Users\user\AppData\Local\node-gyp\Cache\14.1.0\
```

2. Create an empty directory to build and go to this directory:

```shell script
> cmake ../ && \
    cmake --build . --target inference_engine_node --config Release
```

## Build
```sh
$ npm run build
```

**Note:** For debug build on Windows, open the solution in Visual Studio, change library path to "C:\Program Files (x86)\IntelSWTools\openvino\inference_engine\lib\intel64\Debug" and library name to "inference_engined.lib".

### Test

Setup the system environment variables for OpenVINO on [Windows](https://docs.openvinotoolkit.org/2020.1/_docs_install_guides_installing_openvino_windows.html#set-the-environment-variables) and [Linux](https://docs.openvinotoolkit.org/2020.1/_docs_install_guides_installing_openvino_linux.html#set-the-environment-variables).

```sh
$ npm test
```

### Example

Setup system environment variables of OpenVINO as test.

 * [Image Classification in Electron.js](example/hello_classification_electron/README.md)
 * [Image Classification in Node.js](example/hello_classification_node/README.md)
 * [Object Detection in Node.js](example/hello_object_detection_ssd_node/README.md)

### API

[API Doc](doc/api.md)

### Development

[Coding Style](doc/coding_style.md)
