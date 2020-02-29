# Inference Engine Binding for Node.js*

*Other names and brands may be claimed as the property of others.

### Prerequisites

Instal [Node.js](https://nodejs.org/).

Download [OpenVINO](https://software.intel.com/en-us/openvino-toolkit/choose-download/) and install it into the default path.

For Windows, install [Visual Studio 2019](https://visualstudio.microsoft.com/vs/).

For Linux, install `build-essential` package.

**Note:** Node.js 12 LTS is verified.

**Note:** OpenVINO 2019 R3.1 is verified on Windows 10 and Ubuntu Linux 16.04.

### Install

```sh
$ npm install
```

### Build

```sh
$ npm install -g node-gyp
$ node-gyp rebuild
```

**Note:** node-gyp v6.10 and VS2019 are verified on Windows.

**Note:** node-gyp v6.10 and gcc 5.4.0 are verified on Linux.

**Note:** for Debug build on Windows, open the solution in Visual Studio, change library path to "C:\Program Files (x86)\IntelSWTools\openvino\inference_engine\lib\intel64\Debug" and library name to "inference_engined.lib".

### Test

Before run following script, setup the system environment variables for OpenVINO on [Windows](https://docs.openvinotoolkit.org/2019_R3.1/_docs_install_guides_installing_openvino_windows.html#set-the-environment-variables) and [Linux](https://docs.openvinotoolkit.org/2019_R3.1/_docs_install_guides_installing_openvino_linux.html#set-the-environment-variables).

Run test scripts under `test` folder, for example
```sh
$ node test/version.js
```

### Example

Setup system environment variables of OpenVINO as test.

[hello_classification_node](example/hello_classification_node/README.md)

### API

[API Doc](doc/api.md)

### Development

[Coding Style](doc/coding_style.md)