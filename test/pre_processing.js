const describe = require('mocha').describe;
var chai = require('chai');
var chaiAsPromised = require('chai-as-promised');
chai.use(chaiAsPromised);
var expect = chai.expect;

const model_path =
    '/home/user/Developer/repositories/inference-engine-node/test/data/simple.xml';
const ie = require('../lib/inference-engine-node');
const core = new ie.Core();

describe('PreProcessingChannel Test', function() {
  it('Subtraction of mean value from the input', async () => {
    const net = await core.readNetwork(model_path)

    const inputInfo = net.getInputsInfo()[0];

    const meanValue = -5;

    const preProcessInfo = inputInfo.getPreProcess();
    preProcessInfo.init(3);
    preProcessInfo.getPreProcessChannel(0).meanValue = meanValue;
    preProcessInfo.getPreProcessChannel(1).meanValue = meanValue;
    preProcessInfo.getPreProcessChannel(2).meanValue = meanValue;
    preProcessInfo.getPreProcessChannel(0).stdScale = 1;
    preProcessInfo.getPreProcessChannel(1).stdScale = 1;
    preProcessInfo.getPreProcessChannel(2).stdScale = 1;
    preProcessInfo.setVariant('mean_value');

    const execNet = await core.loadNetwork(net, 'CPU');

    const inferReq = await execNet.createInferRequest();
    const inputBlob = inferReq.getBlob(inputInfo.name());

    const inputData = new Float32Array(inputBlob.wmap());

    for (let i = 0; i < inputData.length; i++) {
      inputData[i] = i;
    }

    inputBlob.unmap();

    inferReq.infer();

    const outputInfo = net.getOutputsInfo()[0];
    const outputBlob = inferReq.getBlob(outputInfo.name());
    const outputData = new Float32Array(outputBlob.rmap());

    outputBlob.unmap();

    expect(outputData).to.eql(inputData.map((num) => {
      return num - meanValue;
    }));
  });


  it.skip('Multiplication the input by stdScale value', async () => {
    const net = await core.readNetwork(model_path)

    const inputInfo = net.getInputsInfo()[0];

    const scaleValue = 2;

    const preProcessInfo = inputInfo.getPreProcess();
    preProcessInfo.init(3);
    preProcessInfo.getPreProcessChannel(0).stdScale = scaleValue;
    preProcessInfo.getPreProcessChannel(1).stdScale = scaleValue;
    preProcessInfo.getPreProcessChannel(2).stdScale = scaleValue;
    preProcessInfo.getPreProcessChannel(0).meanValue = 0;
    preProcessInfo.getPreProcessChannel(1).meanValue = 0;
    preProcessInfo.getPreProcessChannel(2).meanValue = 0;

    preProcessInfo.setVariant('mean_value');

    const execNet = await core.loadNetwork(net, 'CPU');

    const inferReq = await execNet.createInferRequest();
    const inputBlob = inferReq.getBlob(inputInfo.name());

    const inputData = new Float32Array(inputBlob.wmap());

    for (let i = 0; i < inputData.length; i++) {
      inputData[i] = i;
    }

    inputBlob.unmap();

    inferReq.infer();

    const outputInfo = net.getOutputsInfo()[0];
    const outputBlob = inferReq.getBlob(outputInfo.name());
    const outputData = new Float32Array(outputBlob.rmap());

    outputBlob.unmap();

    expect(outputData).to.eql(inputData.map((num) => {
      return num * scaleValue;
    }));
  });
});