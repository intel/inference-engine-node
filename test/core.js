const ie = require('../lib/ie');
const model_path = './models/squeezenet1.1/FP16/squeezenet1.1';

const core = ie.createCore();
console.log(core.getVersions('CPU'));

ie.createNetwork(model_path + '.xml', model_path + '.bin')
    .then(net => {
      console.log('IE core load the network:');
      console.log('---------------------------');
      core.loadNetwork(net, 'CPU')
          .then(exec_net => {
            console.log('---------------------------');
            console.log(exec_net);
            console.log('Use class ExecutableNetwork to test the loadNetwork:');
            core.loadNetwork(exec_net, 'CPU');
          })
          .catch(error => {
            console.log(error);
          });
    })
    .catch(error => {
      console.log(error);
    });