# A binding to Inference Engine for Node.JS

### Prerequisites

Dwonload [OpenVINO](https://software.intel.com/en-us/openvino-toolkit/choose-download/) and install into default path.

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

You can use these commands for Linux build.

**Note:** node-gyp v6.10 and VS2019 are verified on Windows.

**Note:** node-gyp v6.10 and gcc 5.4.0 are verified on Linux.

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
Excute the command to set the environment variables
```
source /opt/intel/openvino/bin/setupvars.sh
```

Run script to install external software dependencies
```
cd /opt/intel/openvino/install_dependencies
sudo -E ./install_openvino_dependencies.sh
```

```sh
$ node test/version.js
$ node test/network.js
```

### Example

Setup system environment variables of OpenVINO as test.

```sh
$ cd example
$ npm install
$ npm start
```
