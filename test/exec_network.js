const ie = require('../lib/ie');
const model_path = './models/squeezenet1.1/FP16/squeezenet1.1';

function showInfo(info) {
  console.log(`  name: ${info.name()}`);
  console.log(`  precision: ${info.getPrecision()}`);
  console.log(`  layout: ${info.getLayout()}`);
  console.log(`  dims: [${info.getDims()}]`);
}

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
      const output_info = outputs_info[0];
      console.log('---------------------------');
      console.log('Check CPU plugin version');
      console.log(core.getVersions('CPU'));
      console.log('---------------------------');
      core.loadNetwork(net, 'CPU')
          .then(exec_net => {
            try {
              console.log(`Succeed to load network to CPU plugin`);
              const infer_req = exec_net.createInferRequest();
              console.log('Succeed to create infer request')
              const input_blob = infer_req.getBlob(input_info.name());
              console.log(`Input blob '${input_info.name()}' byte size ${
                  input_blob.byteSize()}`);
              const input_data = new Float32Array(input_blob.buffer());
              for (let i = 0; i < input_blob.size(); i++) {
                input_data[i] = i / (input_blob.size() + 1);
              }
              infer_req.startAsync()
                  .then(() => {
                    console.log('Succeed to infer');
                    const output_blob = infer_req.getBlob(output_info.name());
                    console.log(
                        `Output blob '${output_info.name()}' byte size ${
                            output_blob.byteSize()}`);
                    const output_data = new Float32Array(output_blob.buffer());
                    // Reference results from C++ sample
                    // output_data[0]: 0.000021
                    // output_data[1]: 0.000089
                    // output_data[2]: 0.000050
                    // output_data[3]: 0.000424
                    // output_data[4]: 0.006300
                    if (Math.abs(output_data[0] - 0.000021) < 1e-6) {
                      console.log('Succeed to verify output_data[0]');
                    } else {
                      console.log('Fail to verify output_data[0]');
                    }
                    for (let i = 0; i < 5; i++) {
                      console.log(`output_data[${i}]: ${output_data[i]}`);
                    }
                    console.log('---------------------------');
                    console.log('Done');
                  })
                  .catch(error => {
                    console.log(error);
                  });
              console.log('Do something else here');
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