# An Node.js example of image classification using inference-engine-node

### Install

```sh
$ npm install
```

### Run

```sh
$ node main.js -m <model_path> -i <image_path> -d <device_name>
```

For example on Windows, run SqueezeNet on CPU plugin:
```sh
$ node main.js -m ..\..\models\squeezenet1.1\FP16\squeezenet1.1 -i "c:\Program Files (x86)\IntelSWTools\openvino\deployment_tools\demo\car.png" -d CPU
```