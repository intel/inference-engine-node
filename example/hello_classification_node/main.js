const ie = require('../../lib/ie');
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
    name: 'device',
    alias: 'd',
    type: String,
    defaultValue: 'CPU',
    description:
        'Optional. Specify the target device to infer on; CPU, GPU, FPGA, ' +
        'HDDL, MYRIAD or HETERO: is acceptable. Default value is CPU'
  }
];

const commandLineArgs = require('command-line-args');
const commandLineUsage = require('command-line-usage');

const options = commandLineArgs(option_definitions);
if (options.help || !options.image || !options.model) {
  const usage = commandLineUsage([
    {
      header: 'Hello Classification',
      content: 'An example of image classification using inference-engine-node.'
    },
    {header: 'Options', optionList: option_definitions}
  ])
  console.log(usage)
  process.exit(0);
}

const model_path = options.model;
const re = /\.xml$/g
const bin_path = model_path.replace(re, '.bin');
const labels_path = model_path.replace(re, '.labels');
const device_name = options.device;
const image_path = options.image;

function showInfo(info) {
  console.log(`  name: ${info.name()}`);
  console.log(`  precision: ${info.getPrecision()}`);
  console.log(`  layout: ${info.getLayout()}`);
  console.log(`  dims: [${info.getDims()}]`);
}

function topResults(tensor, labels, k = 5) {
  let probs = Array.from(tensor);
  let indexes = probs.map((prob, index) => [prob, index]);
  let sorted = indexes.sort((a, b) => {
    if (a[0] === b[0]) {
      return 0;
    }
    return a[0] < b[0] ? -1 : 1;
  });
  sorted.reverse();
  let classes = [];
  for (let i = 0; i < k; ++i) {
    const prob = sorted[i][0];
    const index = sorted[i][1];
    const c = {
      label: labels === null ? index : labels[index],
      prob: prob.toFixed(6)
    };
    classes.push(c);
  }
  return classes;
}

console.log(`Inference Engine version: `);
console.log(ie.getVersion());

async function main() {
  const image = await jimp.read(image_path);
  const core = ie.createCore();
  let start_time = performance.now();
  let net = await ie.createNetwork(model_path, bin_path);
  const create_network_time = performance.now() - start_time;
  console.log(`Succeed to create network at ${model_path} took ${
      create_network_time.toFixed(2)} ms`);
  console.log(`Network name: ${net.getName()}`);
  console.log('Inputs info:')
  const inputs_info = net.getInputsInfo();
  inputs_info.forEach(info => {
    showInfo(info);
  });
  console.log('Outputs info:');
  const outputs_info = net.getOutputsInfo();
  outputs_info.forEach(info => {
    showInfo(info);
  });
  const input_info = inputs_info[0];
  console.log(`Change input layout to 'nhwc' and precision to 'u8'`);
  input_info.setLayout('nhwc');
  input_info.setPrecision('u8');
  showInfo(input_info);
  const output_info = outputs_info[0];
  console.log('---------------------------');
  console.log(`Check ${device_name} plugin version`);
  console.log(core.getVersions(device_name));
  console.log('---------------------------');
  start_time = performance.now();
  const exec_net = await core.loadNetwork(net, device_name);
  const load_network_time = performance.now() - start_time;
  console.log(`Succeed to load network to ${device_name} plugin took ${
      load_network_time.toFixed(2)} ms`);
  let infer_req = exec_net.createInferRequest();
  const input_blob = infer_req.getBlob(input_info.name());
  const input_dims = input_info.getDims();
  const input_height = input_dims[2];
  const input_width = input_dims[3];
  const input_channels = input_dims[1];
  const input_data = new Uint8Array(input_blob.buffer());
  image.resize(input_width, input_height, jimp.RESIZE_BILINEAR);
  console.log(`Prepare image with height = ${image.bitmap.width} width = ${
      image.bitmap.height}`);
  image.scan(
      0, 0, image.bitmap.width, image.bitmap.height, function(x, y, idx) {
        // RGBA to GBR
        let i = Math.floor(idx / 4) * 3;
        input_data[i + 2] = image.bitmap.data[idx + 0];  // R
        input_data[i + 0] = image.bitmap.data[idx + 1];  // G
        input_data[i + 1] = image.bitmap.data[idx + 2];  // B
      });
  console.log('---------------------------');
  start_time = performance.now();
  await infer_req.startAsync();
  const infer_time = performance.now() - start_time;
  console.log(`Succeed to infer took ${infer_time.toFixed(2)} ms`);
  const output_blob = infer_req.getBlob(output_info.name());
  const output_data = new Float32Array(output_blob.buffer());
  const data = await fs.readFile(labels_path, {encoding: 'utf-8'});
  labels = data.split('\n');
  const results = topResults(output_data, labels);
  console.log('Results:')
  console.log(results);
  console.log('---------------------------');
  return 'Done';
}

main().then(console.log).catch(console.error);
