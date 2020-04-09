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

  -h, --help                Show this help message and exit.
  -m, --model string        Required. Path to an .xml file with a trained model.
  -i, --image string        Required. Path to an image file.
  -d, --device string       Optional. Specify the target device to infer on; CPU, GPU, FPGA, HDDL, MYRIAD
                            or HETERO: is acceptable. Default value is CPU.
  -n, --iterations number   Optional. The number of iterations for inference. Default value is 1.
  -k, --topk number         Optional. The number of top results to show. Default value is 5.
  -s, --sync                Optional. Specify to inference synchronously or asynchronously. Default value
                            is false.
```

### Run

For example on Windows, run SqueezeNet on CPU plugin for 10 iterations:
```sh
$ node main.js -m ..\..\models\squeezenet1.1\FP16\squeezenet1.1.xml -i test.png -d CPU -n 10

Start.
-------------------------------------------
Check inference engine version:
  API version: 2.1
  Build: 37988
  Description: API
-------------------------------------------
Start to create network from ..\..\models\squeezenet1.1\FP16\squeezenet1.1.xml.
Succeeded: read network took 7.13 ms.
Network name: squeezenet1.1
Input[0]:
  name: data
  precision: fp32
  layout: nchw
  dims: [1,3,227,227]
Output[0]:
  name: prob
  precision: fp32
  layout: nchw
  dims: [1,1000,1,1]
Change input layout to 'nhwc' and precision to 'u8'.
-------------------------------------------
Start to read image from test.png.
Succeeded.
-------------------------------------------
Check CPU plugin version:
  Deivce Name: CPU
  API version: 2.1
  Build: 37988
  Description: MKLDNNPlugin
Start to load network to CPU plugin.
Succeeded: load network took 147.38 ms.
-------------------------------------------
Start to infer asynchronously for 10 iterations.
Succeeded: the average inference time is 6.30 ms.
           the throughput is 158.73 FPS.
The top 5 results:
classid   probability    label
-------   -------        -------
387       0.998859       lesser panda, red panda, panda, bear cat, cat bear, Ailurus fulgens
294       0.000253       brown bear, bruin, Ursus arctos
277       0.000243       red fox, Vulpes vulpes
278       0.000180       kit fox, Vulpes macrotis
298       0.000084       mongoose
-------------------------------------------
Done.
```