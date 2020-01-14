const ie = require('../lib/ie');
const model_path = './models/squeezenet1.1/FP16/squeezenet1.1';

const core = ie.createCore();
console.log(core.getVersions('CPU'));

ie.createNetwork(model_path + '.xml', model_path + '.bin')
    .then(net => {
      console.log('IE core load the network:');
      core.loadNetwork(net, 'CPU')
          .then(exec_net => {
            console.log('Name of ExecNetwork:')
            console.log(exec_net.getName());
          })
          .catch(error => {
            console.log(error);
          });
    })
    .catch(error => {
      console.log(error);
    });