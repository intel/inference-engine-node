const ie = require('../lib/ie');
const model_path = './models/squeezenet1.1/FP16/squeezenet1.1';
ie.createNetwork(model_path + '.xml', model_path + '.bin')
  .then(net => {
    console.log(`Succeed to load network at ${model_path}`);
    console.log(net);
    console.log(`Name: ${net.getName()}`);
  }).catch(error => {
    console.log(error);
  });