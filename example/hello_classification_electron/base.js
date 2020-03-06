
const modelZoo = {

  imageClassificationModels: [
    {
      modelName: 'SqueezeNet (OpenVino)',
      format: 'OpenVINO',
      modelId: 'squeezenet_openvino',
      modelSize: '4.9MB',
      modelFile: './model/squeezenet1.1.bin',
      labelsFile: './model/labels1000.txt',
      onlineFile: '',
      onlineBinFile: '',
      inputSize: [227, 227, 3],
      outputSize: 1000,
      preOptions: {
        channelScheme: 'BGR',
      },
      intro:
          'A light-weight CNN providing Alexnet level accuracy with 50X fewer parameters.',
      paperUrl: 'https://arxiv.org/abs/1602.07360'
    },
    {
      modelName: 'MobileNet v1 (OpenVino)',
      format: 'OpenVINO',
      modelId: 'mobilenet_v1_openvino',
      modelSize: '16.9MB',
      modelFile: './model/mobilenet_v1_1.0_224.bin',
      labelsFile: './model/labels1001.txt',
      onlineFile: '',
      onlineBinFile: '',
      inputSize: [224, 224, 3],
      outputSize: 1001,
      preOptions: {
        mean: [127.5, 127.5, 127.5],
        std: [127.5, 127.5, 127.5],
      },
      intro:
          'An efficient Convolutional Neural Networks for Mobile Vision Applications.',
      paperUrl: 'https://arxiv.org/pdf/1704.04861.pdf'
    },
    {
      modelName: 'MobileNet v2 (OpenVino)',
      format: 'OpenVINO',
      modelId: 'mobilenet_v2_openvino',
      modelSize: '14.0MB',
      modelFile: './model/mobilenet_v2_1.0_224.bin',
      labelsFile: './model/labels1001.txt',
      onlineFile: '',
      onlineBinFile: '',
      inputSize: [224, 224, 3],
      outputSize: 1001,
      preOptions: {
        mean: [127.5, 127.5, 127.5],
        std: [127.5, 127.5, 127.5],
      },
      intro:
          'MobileNetV2 improves the state of the art performance of mobile models.',
      paperUrl: 'https://arxiv.org/abs/1801.04381'
    },
    {
      modelName: 'ResNet50 v1 (OpenVino)',
      format: 'OpenVINO',
      modelId: 'resnet50_v1_openvino',
      modelSize: '102.1MB',
      modelFile: './model/resnet-50.bin',
      labelsFile: './model/labels1000.txt',
      onlineFile: '',
      onlineBinFile: '',
      inputSize: [224, 224, 3],
      outputSize: 1000,
      preOptions: {
        channelScheme: 'BGR',
      },
      intro:
          'A residual learning framework to ease the training of networks that are substantially deeper than those used previously. This result won the 1st place on the ILSVRC 2015 classification task.',
      paperUrl: 'https://arxiv.org/abs/1512.03385'
    },
    {
      modelName: 'DenseNet 121 (OpenVino)',
      format: 'OpenVINO',
      modelId: 'densenet_121_openvino',
      modelSize: '31.9MB',
      modelFile: './model/densenet-121.bin',
      labelsFile: './model/labels1000.txt',
      onlineFile: '',
      onlineBinFile: '',
      inputSize: [224, 224, 3],
      outputSize: 1000,
      preOptions: {
        mean: [0.406, 0.456, 0.485],
        std: [0.225, 0.224, 0.229],
        norm: true,
        channelScheme: 'BGR',
      },
      postOptions: {
        softmax: true,
      },
      intro:
          'Dense Convolutional Network (DenseNet) connects each layer to every other layer in a feed-forward fashion. DenseNets have several compelling advantages: they alleviate the vanishing-gradient problem, strengthen feature propagation, encourage feature reuse, and substantially reduce the number of parameters. ',
      paperUrl: 'https://arxiv.org/abs/1608.06993'
    }
  ]
};

// extract model lists into the browser env for backward compatiblity
for (const modelListName in modelZoo) {
  window[modelListName] = modelZoo[modelListName];
}

const getSearchParamsBackend = () => {
  let searchParams = new URLSearchParams(location.search);
  return searchParams.has('b') ? searchParams.get('b') : '';
} 

const getSearchParamsModel = () => {
  let searchParams = new URLSearchParams(location.search);
  return searchParams.has('m') ? searchParams.get('m') : 'none';
} 

const getSearchParamsPlugin = () => {
  let searchParams = new URLSearchParams(location.search);
  return searchParams.has('plugin') ? searchParams.get('plugin') : 'none';
}
