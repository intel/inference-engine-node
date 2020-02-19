const ie = require('../../lib/ie');
const jimp = require('jimp');
const fs = require('fs');

const option_definitions = [
  {name: 'model', alias: 'm', type: String},
  {name: 'image', alias: 'i', type: String},
  {name: 'device', alias: 'd', type: String, defaultValue: 'CPU'}
];
const commandLineArgs = require('command-line-args');
const options = commandLineArgs(option_definitions);

const model_path = options.model;
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

jimp.read(image_path)
    .then(image => {
      const core = ie.createCore();

      ie.createNetwork(model_path + '.xml', model_path + '.bin')
          .then(net => {
            console.log(`Succeed to create network at ${model_path}`);
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
            core.loadNetwork(net, device_name)
                .then(exec_net => {
                  try {
                    console.log(
                        `Succeed to load network to ${device_name} plugin`);
                    const infer_req = exec_net.createInferRequest();
                    console.log('Succeed to create infer request')
                    const input_blob = infer_req.getBlob(input_info.name());
                    const input_dims = input_info.getDims();
                    const input_height = input_dims[2];
                    const input_width = input_dims[3];
                    const input_channels = input_dims[1];
                    const input_data = new Uint8Array(input_blob.buffer());
                    image.resize(
                        input_width, input_height, jimp.RESIZE_BILINEAR);
                    console.log(`Prepare image with height = ${
                        image.bitmap.width} width = ${image.bitmap.height}`);
                    image.scan(
                        0, 0, image.bitmap.width, image.bitmap.height,
                        function(x, y, idx) {
                          // RGBA to GBR
                          let i = Math.floor(idx / 4) * 3;
                          input_data[i + 2] = image.bitmap.data[idx + 0];  // R
                          input_data[i + 0] = image.bitmap.data[idx + 1];  // G
                          input_data[i + 1] = image.bitmap.data[idx + 2];  // B
                        });
                    infer_req.startAsync()
                        .then(() => {
                          console.log('---------------------------');
                          console.log('Succeed to infer');
                          const output_blob =
                              infer_req.getBlob(output_info.name());
                          const output_data =
                              new Float32Array(output_blob.buffer());
                          fs.readFile(
                              model_path + '.labels', {encoding: 'utf-8'},
                              function(err, data) {
                                let labels = null;
                                if (!err) {
                                  labels = data.split('\n');
                                }
                                const results = topResults(output_data, labels);
                                console.log('Results:')
                                console.log(results);
                                console.log('---------------------------');
                                console.log('Done');
                              });
                        })
                        .catch(error => {
                          console.log(error);
                        });
                  } catch (error) {
                    console.log(error);
                  }
                })
                .catch(error => {
                  console.log(error);
                });
          })
          .catch(error => {
            console.log(error);
          });
    })
    .catch(err => {
      console.error(err);
    });