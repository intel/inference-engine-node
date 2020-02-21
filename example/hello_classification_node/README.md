# A Node.js example of image classification using inference-engine-node

### Install

```sh
$ npm install
```

### Usage

```sh
$ node main.js --help

Hello Classification

  An example of image classification using inference-engine-node.

Options

  -h, --help            Show this help message and exit.
  -m, --model string    Required. Path to an .xml file with a trained model.
  -i, --image string    Required. Path to an image file.
  -d, --device string   Optional. Specify the target device to infer on; CPU, GPU, FPGA, HDDL, MYRIAD
                        or HETERO: is acceptable. Default value is CPU
```

### Run

For example on Windows, run SqueezeNet on CPU plugin:
```sh
$ node main.js -m ..\..\models\squeezenet1.1\FP16\squeezenet1.1.xml -i "c:\Program Files (x86)\IntelSWTools\openvino\deployment_tools\demo\car.png" -d CPU
```