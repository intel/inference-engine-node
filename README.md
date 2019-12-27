# A binding to Inference Engine for Node.JS

### Install

```sh
$ npm install
```

### Build

Note: only support building on Linux.

Make sure the [OpenVINO](https://software.intel.com/en-us/openvino-toolkit/choose-download/free-download-linux) is installed into default path (/opt/intel/computer_vision_sdk/).

```sh
$ npm run-script build
```

### Test

```sh
$ LD_LIBRARY_PATH=/opt/intel/computer_vision_sdk/inference_engine/lib/ubuntu_16.04/intel64/ node test/version.js
```
