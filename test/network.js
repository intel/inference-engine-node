const ie = require('../lib/ie');
const model_path = './models/squeezenet1.1/FP16/squeezenet1.1';

function showInputInfo(info) {
  console.log(`  name: ${info.name()}`);
  console.log(`  precision: ${info.getPrecision()}`);
  console.log(`  layout: ${info.getLayout()}`);
  console.log(`  dims: [${info.getDims()}]`);
}

ie.createNetwork(model_path + '.xml', model_path + '.bin')
  .then(net => {
    console.log(`Succeed to load network at ${model_path}`);
    console.log(net);
    console.log(`Network name: ${net.getName()}`);
    console.log('Inputs info:')
    const inputs_info = net.getInputsInfo();
    inputs_info.forEach(info => {
      showInputInfo(info);
    });
    let info = inputs_info[0];
    console.log(`Set input ${info.name()} layout to 'nhwc'`);
    info.setLayout('nhwc');
    console.log(`Set input ${info.name()} precision to 'u8'`);
    info.setPrecision('u8');
    console.log(`Input info: `);
    showInputInfo(info);
  }).catch(error => {
    console.log(error);
  });