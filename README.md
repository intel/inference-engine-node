# A binding to Inference Engine for Node.JS

### Prerequisites

Download [OpenVINO](https://software.intel.com/en-us/openvino-toolkit/choose-download/) and install into the default path.

**Note:** Windows and Linux are supproted currently.

**Note:** OpenVINO 2019 R3.1 is verified on Windows and Linux.

### Install

```sh
$ npm install
```

### Build

For Windows build, please install Visual Studio 2019.

```sh
$ npm install -g node-gyp
$ node-gyp rebuild
```

**Note:** node-gyp v6.10 and VS2019 are verified on Windows.

**Note:** node-gyp v6.10 and gcc 5.4.0 are verified on Linux.

**Note:** for Debug build on Windows, please change library path to "C:\Program Files (x86)\IntelSWTools\openvino\inference_engine\lib\intel64\Debug" and library name to "inference_engined.lib".

### Test

Before run following script, setup the system environment variables for OpenVINO on [Windows](https://docs.openvinotoolkit.org/2019_R3.1/_docs_install_guides_installing_openvino_windows.html#set-the-environment-variables) and [Linux](https://docs.openvinotoolkit.org/2019_R3.1/_docs_install_guides_installing_openvino_linux.html#set-the-environment-variables).

For example, on Windows, execute
```sh
C:\> "c:\Program Files (x86)\IntelSWTools\openvino\bin\setupvars.bat"
```

On Linux, make sure that OpenVINO is installed at
```
/opt/intel/
```
Execute the command to setup the environment variables
```
source /opt/intel/openvino/bin/setupvars.sh
```

Run script to install external software dependencies
```
cd /opt/intel/openvino/install_dependencies
sudo -E ./install_openvino_dependencies.sh
```

Run tests
```sh
$ node test/version.js
$ node test/network.js
```

### Example

Setup system environment variables of OpenVINO as test.

[hello_classification_node](example/hello_classification_node/README.md)

### Coding style

Use `clang-format` and follow [Chromium coding style](https://chromium.googlesource.com/chromium/src/+/master/styleguide/c++/c++.md).

```sh
$ npm install -g clang-format
```

For C++ code, please refer to [Using clang-format on C++ Code](https://electronjs.org/docs/development/clang-format).
```sh
$ clang-format -i src/binding.cc
```

For JS code, please refer to [Formatting .js with clang-format](https://github.com/google/closure-library/wiki/Formatting-.js-with-clang-format).

```sh
$ clang-format -i -style=Google test/network.js
```