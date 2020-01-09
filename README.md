# A binding to Inference Engine for Node.JS

### Prerequisites

Dwonload [OpenVINO](https://software.intel.com/en-us/openvino-toolkit/choose-download/) and install into default path.

Note: currently only support Windows and Linux.

### Install

```sh
$ npm install
```

### Build

```sh
$ npm install -g node-gyp
$ node-gyp build
```

Note: verified for node-gyp v6.10 and VS2019

### Test

Before run following script, setup the system environment variables for OpenVINO on [Windows](https://docs.openvinotoolkit.org/2019_R3.1/_docs_install_guides_installing_openvino_windows.html#set-the-environment-variables) and [Linux](https://docs.openvinotoolkit.org/2019_R3.1/_docs_install_guides_installing_openvino_linux.html#set-the-environment-variables).

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
