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
      console.log('---------------------------');
      console.log('Check CPU plugin version');
      console.log(core.getVersions('CPU'));
      console.log('---------------------------');
      core.loadNetwork(net, 'CPU')
          .then(exec_net => {
            try {
              console.log(`Succeed to load network to CPU plugin`);
              console.log(exec_net);
              const infer_req = exec_net.createInferRequest();
              console.log('Succeed to crate infer request')
              console.log(infer_req);
              const blob = infer_req.getBlob(input_info.name());
              console.log(blob);
              console.log(
                  `Blob '${input_info.name()}' byte size ${blob.byteSize()}`);
              // TODO: set input blob
              infer_req.startAsync()
                  .then(
                      () => {
                          // TODO: get output blob
                      })
                  .catch(error => {
                    console.log(error);
                  })
              console.log('---------------------------');
              console.log('Done');
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