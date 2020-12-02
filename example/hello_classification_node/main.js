const {Core, postProcessing, getVersion} = require('../../lib/inference-engine-node');

const {
  binPathFromXML,
  classification,
  showAvailableDevices,
  warning,
  showBreakLine,
  showVersion,
  highlight,
  showInputOutputInfo,
  showPluginVersions,
  labelsPathFromXML
} = require('../common');

const jimp = require('jimp');
const fs = require('fs').promises;
const {performance} = require('perf_hooks');

const option_definitions = [
  {
    name: 'help',
    alias: 'h',
    type: Boolean,
    description: 'Show this help message and exit.'
  },
  {
    name: 'model',
    alias: 'm',
    type: String,
    description: 'Required. Path to an .xml file with a trained model.'
  },
  {
    name: 'image',
    alias: 'i',
    type: String,
    description: 'Required. Path to an image file.'
  },
  {
    name: 'labels',
    alias: 'l',
    type: String,
    description: 'Optional. Path to a label file.'
  },
  {
    name: 'device',
    alias: 'd',
    type: String,
    defaultValue: 'CPU',
    description: 'Optional. Specify the target device to infer on ' +
        '(the list of available devices is shown below). ' +
        'Default value is CPU.'
  },
  {
    name: 'iterations',
    alias: 'n',
    type: Number,
    defaultValue: 1,
    description: 'Optional. The number of iterations for inference. ' +
        'Default value is 1.'
  },
  {
    name: 'topk',
    alias: 'k',
    type: Number,
    defaultValue: 5,
    description: 'Optional. The number of top results to show. ' +
        'Default value is 5.'
  },
  {
    name: 'sync',
    alias: 's',
    type: Boolean,
    defaultValue: false,
    description:
        'Optional. Specify to inference synchronously or asynchronously. ' +
        'Default value is false.'
  },
  {
    name: 'mean',
    type: String,
    defaultValue: '[0,0,0]',
    description: 'Optional. Specify the mean value for input channels. ' +
        'Default value is "[0,0,0]"'
  },
  {
    name: 'std',
    type: String,
    defaultValue: '[1,1,1]',
    description: 'Optional. Specify the std scale value for input channels. ' +
        'Default value is "[1,1,1]"'
  },
  {
    name: 'color',
    type: String,
    defaultValue: 'bgr',
    description:
        'Optional. Specify the color format for input, "bgr" or "rgb". ' +
        'Default value is "bgr"'
  }
];

const commandLineArgs = require('command-line-args');
const commandLineUsage = require('command-line-usage');

async function main() {
  const core = new Core();

  const options = commandLineArgs(option_definitions);
  if (options.help || !options.image || !options.model) {
    const usage = commandLineUsage([
      {
        header: 'Hello Classification',
        content:
            'An example of image classification using inference-engine-node.'
      },
      {header: 'Options', optionList: option_definitions}
    ]);
    console.log(usage);
    showAvailableDevices(core);
    process.exit(0);
  }

  const model_path = options.model;
  const bin_path = binPathFromXML(model_path)
  const labels_path =
      options.labels ? options.labels : labelsPathFromXML(model_path)
  const device_name = options.device;
  const image_path = options.image;
  const iterations = options.iterations;
  const top_k = options.topk;
  const sync = options.sync;

  if (iterations <= 0) {
    warning('The number of iterations should be greater than 0.');
    process.exit(-1);
  }

  let mean, std;
  try {
    mean = JSON.parse(options.mean);
    std = JSON.parse(options.std);
  } catch (error) {
    warning(`Error to parse mean or std: ${error}`);
    process.exit(-1);
  }

  if (!(mean instanceof Array)) {
    warning('The mean is not an array.');
    process.exit(-1);
  } else if (mean.length !== 3) {
    warning('The length of mean is not 3.');
    process.exit(-1);
  }
  if (!(std instanceof Array)) {
    warning('The std is not an array.');
    process.exit(-1);
  } else if (std.length !== 3) {
    warning('The length of std is not 3.');
    process.exit(-1);
  }
  const preprocess =
      !(mean[0] === 0 && mean[1] === 0 && mean[2] === 0 && std[0] === 1 &&
        std[1] === 1 && std[2] === 1);
  const color = options.color;

  if (color !== 'bgr' && color !== 'rgb') {
    warning('The color format should be either "bgr" or "rgb".');
    process.exit(0);
  }

  console.log('Start.')
  showBreakLine();

  console.log(`Check inference engine version: `);
  showVersion(getVersion());
  showBreakLine();

  console.log(`Start to create network from ${model_path}.`)

  let start_time = performance.now();
  let net = await core.readNetwork(model_path, bin_path);
  const create_network_time = performance.now() - start_time;
  highlight(
      `Succeeded: read network took ${create_network_time.toFixed(2)} ms.`);
  console.log(`Network name: ${net.getName()}`);

  const inputs_info = net.getInputsInfo();
  inputs_info.forEach((info, i) => {
    console.log(`Input[${i}]:`)
    showInputOutputInfo(info);
  });

  const outputs_info = net.getOutputsInfo();
  outputs_info.forEach((info, i) => {
    console.log(`Output[${i}]:`)
    showInputOutputInfo(info);
  });

  const input_info = inputs_info[0];
  console.log(`Set input layout to 'nhwc'.`);
  input_info.setLayout('nhwc');
  if (!preprocess) {
    console.log(`Set input precision to 'u8'.`);
    input_info.setPrecision('u8');
  }

  const output_info = outputs_info[0];
  showBreakLine();

  console.log(`Start to read image from ${image_path}.`);
  const image = await jimp.read(image_path);

  console.log(`Succeeded.`);

  const input_dims = input_info.getDims();
  const input_height = input_dims[2];
  const input_width = input_dims[3];
  if (image.bitmap.height !== input_height ||
      image.bitmap.width !== input_width) {
    console.log(`Resize image from (${image.bitmap.height}, ${
        image.bitmap.width}) to (${input_height}, ${input_width}).`);
    image.resize(input_width, input_height, jimp.RESIZE_BILINEAR);
  }
  showBreakLine();

  console.log(`Check ${device_name} plugin version:`);
  showPluginVersions(core.getVersions(device_name));

  console.log(`Start to load network to ${device_name} plugin.`)
  start_time = performance.now();
  const exec_net = await core.loadNetwork(net, device_name);
  const load_network_time = performance.now() - start_time;
  highlight(`Succeeded: load network took ${load_network_time.toFixed(2)} ms.`);
  showBreakLine();

  let infer_req;
  let infer_time = [];

  console.log(`Start to infer ${sync ? '' : 'a'}synchronously for ${
      iterations} iterations.`);

  for (let i = 0; i < iterations; i++) {
    start_time = performance.now();
    infer_req = exec_net.createInferRequest();
    const input_blob = infer_req.getBlob(input_info.name());
    let input_data;
    if (!preprocess) {
      input_data = new Uint8Array(input_blob.wmap());
    } else {
      input_data = new Float32Array(input_blob.wmap());
    }

    image.scan(
        0, 0, image.bitmap.width, image.bitmap.height, function(x, y, idx) {
          // Convert from RGBA to BGR (IE default)
          let i = Math.floor(idx / 4) * 3;
          const rgb = color === 'bgr' ? {r: 2, g: 1, b: 0} : {r: 0, g: 1, b: 2};
          input_data[i + rgb.r] =
              (image.bitmap.data[idx + 0] - mean[0]) / std[0];  // R
          input_data[i + rgb.g] =
              (image.bitmap.data[idx + 1] - mean[1]) / std[1];  // G
          input_data[i + rgb.b] =
              (image.bitmap.data[idx + 2] - mean[2]) / std[2];  // B
        });
    input_blob.unmap();
    start_time = performance.now()

    if (sync) {
      infer_req.infer();
    }
    else {
      await infer_req.startAsync();
    }
    infer_time.push(performance.now() - start_time);
  }
  const average_infer_time =
      infer_time.reduce((acc, v) => acc + v, 0) / infer_time.length;

  highlight(`Succeeded: the average inference time is ${
      average_infer_time.toFixed(2)} ms.`);
  highlight(`           the throughput is ${
      (1000 / average_infer_time).toFixed(2)} FPS.`);

  let labels = undefined;

  try {
    const data = await fs.readFile(labels_path, {encoding: 'utf-8'});
    labels = data.split('\n');
  } catch (error) {
    warning(error);
  }
  const output_blob = infer_req.getBlob(output_info.name());
  const output_data = new Float32Array(output_blob.rmap());
  const results =
      postProcessing.topClassificationResults(output_data, labels, top_k);
  output_blob.unmap();
  console.log(`The top ${top_k} results:`);
  classification.showResults(results);
  showBreakLine();
  return 'Done.';
}

main().then(console.log).catch(warning);
