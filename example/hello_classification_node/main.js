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
        'HDDL, MYRIAD or HETERO: is acceptable. Default value is CPU.'
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
  ]);
  console.log(usage);
  process.exit(0);
}

const model_path = options.model;
const re = /\.xml$/g
const bin_path = model_path.replace(re, '.bin');
const labels_path = model_path.replace(re, '.labels');
const device_name = options.device;
const image_path = options.image;
const iterations = options.iterations;
const top_k = options.topk;

if (iterations <= 0) {
  console.log('The number of iterations should be greater than 0.');
  process.exit(0);
}

function showInputOutputInfo(info) {
  console.log(`  name: ${info.name()}`);
  console.log(`  precision: ${info.getPrecision()}`);
  console.log(`  layout: ${info.getLayout()}`);
  console.log(`  dims: [${info.getDims()}]`);
}

function showVersion(version) {
  console.log(
      `  API version: ${version.apiVersion.major}.${version.apiVersion.minor}`);
  console.log(`  Build: ${version.buildNumber}`);
  console.log(`  Description: ${version.description}`);
}

function showPluginVersions(versions) {
  Object.keys(versions).forEach(name => {
    console.log(`  Deivce Name: ${name}`);
    showVersion(versions[name]);
  });
}

function showResults(results) {
  console.log('Class'.padStart(20) + 'Probability'.padStart(20));
  console.log('------------'.padStart(20) + '-----------'.padStart(20));
  results.forEach(result => {
    const label = result.label.split(',')[0].padStart(20);
    const prob = `${(result.prob * 100).toFixed(2)}%`.padStart(20);
    console.log(label + prob);
  })
}

function showBreakline() {
  console.log('-------------------------------------------');
}

function topResults(tensor, labels, k) {
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

const color_green = '\x1b[32m';
const color_reset = '\x1b[0m';

async function main() {
  console.log('Start.')
  showBreakline();
  console.log(`Check inference engine version: `);
  showVersion(ie.getVersion());
  showBreakline();
  const core = ie.createCore();
  console.log(`Start to create network from ${model_path}.`)
  let start_time = performance.now();
  let net = await ie.createNetwork(model_path, bin_path);
  const create_network_time = performance.now() - start_time;
  console.log(`${color_green}Succeeded by ${
      create_network_time.toFixed(2)} ms.${color_reset}`);
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
  console.log(`Change input layout to 'nhwc' and precision to 'u8'.`);
  input_info.setLayout('nhwc');
  input_info.setPrecision('u8');
  const output_info = outputs_info[0];
  showBreakline();
  console.log(`Start to read image from ${image_path}.`);
  const image = await jimp.read(image_path);
  console.log(`Succeeded. The image size is ${image.bitmap.height}x${
      image.bitmap.width}.`);
  const input_dims = input_info.getDims();
  const input_height = input_dims[2];
  const input_width = input_dims[3];
  image.resize(input_width, input_height, jimp.RESIZE_BILINEAR);
  console.log(`Resize image to ${image.bitmap.height}x${image.bitmap.width}.`);
  showBreakline();
  console.log(`Check ${device_name} plugin version:`);
  showPluginVersions(core.getVersions(device_name));
  console.log(`Start to load network to ${device_name} plugin.`)
  start_time = performance.now();
  const exec_net = await core.loadNetwork(net, device_name);
  const load_network_time = performance.now() - start_time;
  console.log(`${color_green}Succeeded by ${load_network_time.toFixed(2)} ms.${
      color_reset}`);
  showBreakline();
  let infer_req;
  let input_time = [];
  let infer_time = [];
  console.log(`Start to inference for ${iterations} iterations.`);
  for (let i = 0; i < iterations; i++) {
    start_time = performance.now();
    infer_req = exec_net.createInferRequest();
    const input_blob = infer_req.getBlob(input_info.name());
    const input_data = new Uint8Array(input_blob.buffer());
    image.scan(
        0, 0, image.bitmap.width, image.bitmap.height, function(x, y, idx) {
          // RGBA to GBR
          let i = Math.floor(idx / 4) * 3;
          input_data[i + 2] = image.bitmap.data[idx + 0];  // R
          input_data[i + 0] = image.bitmap.data[idx + 1];  // G
          input_data[i + 1] = image.bitmap.data[idx + 2];  // B
        });
    input_time.push(performance.now() - start_time);
    start_time = performance.now();
    await infer_req.startAsync();
    infer_time.push(performance.now() - start_time);
  }
  const average_input_time =
      input_time.reduce((acc, v) => acc + v, 0) / input_time.length;
  const average_infer_time =
      infer_time.reduce((acc, v) => acc + v, 0) / infer_time.length;
  console.log(`${color_green}Succeeded by average inference time ${
      average_infer_time.toFixed(2)} ms and input filling time ${
      average_input_time.toFixed(2)} ms.${color_reset}`);
  const output_blob = infer_req.getBlob(output_info.name());
  const output_data = new Float32Array(output_blob.buffer());
  const data = await fs.readFile(labels_path, {encoding: 'utf-8'});
  labels = data.split('\n');
  const results = topResults(output_data, labels, top_k);
  console.log(`The top ${top_k} results:`);
  showResults(results);
  showBreakline();
  return 'Done.';
}

main().then(console.log).catch(console.error);
