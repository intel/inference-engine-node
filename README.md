# Inference Engine Binding for Node.js*

*Other names and brands may be claimed as the property of others.

### Prerequisites

Instal [Node.js](https://nodejs.org/).

Download [OpenVINO](https://software.intel.com/en-us/openvino-toolkit/choose-download/) and install it into the default path.

For Windows, install [Visual Studio 2019](https://visualstudio.microsoft.com/vs/).

For Linux, install `build-essential` package.

**Verified configurations:**
  * Node.js 12 LTS
  * OpenVINO 2019 R3.1
  * Windows 10
  * Ubuntu Linux 16.04

### Install

```sh
$ npm install
```

**Note:** For "Error: MSBuild is not set" on VS 2019, please set `msbuild_path`, e.g.
```
$ npm config set msbuild_path "c:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe"
```
**Note:** For debug build on Windows, open the solution in Visual Studio, change library path to "C:\Program Files (x86)\IntelSWTools\openvino\inference_engine\lib\intel64\Debug" and library name to "inference_engined.lib".

### Test

Setup the system environment variables for OpenVINO on [Windows](https://docs.openvinotoolkit.org/2019_R3.1/_docs_install_guides_installing_openvino_windows.html#set-the-environment-variables) and [Linux](https://docs.openvinotoolkit.org/2019_R3.1/_docs_install_guides_installing_openvino_linux.html#set-the-environment-variables).

```sh
$ npm test
```

### Example

Setup system environment variables of OpenVINO as test.

[hello_classification_node](example/hello_classification_node/README.md)

### API

[API Doc](doc/api.md)

### Development

[Coding Style](doc/coding_style.md)