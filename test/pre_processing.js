const describe = require('mocha').describe;
var chai = require('chai');
var chaiAsPromised = require('chai-as-promised');
chai.use(chaiAsPromised);
var expect = chai.expect;

const model_path = './test/data/simple.xml';
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
  it('Subtraction of mean data from the input', async () => {
    const net = await core.readNetwork(model_path)

    const inputInfo = net.getInputsInfo()[0];

    const preProcessInfo = inputInfo.getPreProcess();
    preProcessInfo.init(3);

    let width = 3;
    let height = 3;
    let typedArray = new Float32Array(width * height);
    const meanValue = -15.0
    typedArray.fill(meanValue);
    let tensorDesc = {precision: 'fp32', dims: [width, height], layout: 'hw'};
    let meanData = {tensorDesc: tensorDesc, data: typedArray.buffer};

    preProcessInfo.getPreProcessChannel(0).meanData = meanData;
    preProcessInfo.getPreProcessChannel(1).meanData = meanData;
    preProcessInfo.getPreProcessChannel(2).meanData = meanData;
    preProcessInfo.setVariant('mean_image');

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

  it('Get mean data', async () => {
    const net = await core.readNetwork(model_path)

    const inputInfo = net.getInputsInfo()[0];

    const preProcessInfo = inputInfo.getPreProcess();
    preProcessInfo.init(3);

    let width = 3;
    let height = 3;
    const meanValue = -2
    const tensorDesc = {precision: 'fp32', dims: [width, height], layout: 'hw'};

    let typedArray0 = new Float32Array(width * height);
    typedArray0.fill(meanValue * 1);
    preProcessInfo.getPreProcessChannel(0).meanData = {tensorDesc: tensorDesc, data: typedArray0.buffer};

    let typedArray1 = new Float32Array(width * height);
    typedArray1.fill(meanValue * 2);
    preProcessInfo.getPreProcessChannel(1).meanData = {tensorDesc: tensorDesc, data: typedArray1.buffer};

    let typedArray2 = new Float32Array(width * height);
    typedArray2.fill(meanValue * 3);
    preProcessInfo.getPreProcessChannel(2).meanData = {tensorDesc: tensorDesc, data: typedArray2.buffer};
    preProcessInfo.setVariant('mean_image');

    const meanData0 = preProcessInfo.getPreProcessChannel(0).meanData
    expect(meanData0.tensorDesc).to.eql(tensorDesc)
    expect(new Float32Array(meanData0.data)).to.eql(typedArray0)

    const meanData1 = preProcessInfo.getPreProcessChannel(1).meanData
    expect(meanData1.tensorDesc).to.eql(tensorDesc)
    expect(new Float32Array(meanData1.data)).to.eql(typedArray1)

    const meanData2 = preProcessInfo.getPreProcessChannel(2).meanData
    expect(meanData1.tensorDesc).to.eql(tensorDesc)
    expect(new Float32Array(meanData2.data)).to.eql(typedArray2)
  });

  it('Subtraction of mean data from the input per channel', async () => {
    const net = await core.readNetwork(model_path)

    const inputInfo = net.getInputsInfo()[0];

    const preProcessInfo = inputInfo.getPreProcess();
    preProcessInfo.init(3);

    let width = 3;
    let height = 3;
    const meanValue = -1
    const tensorDesc = {precision: 'fp32', dims: [width, height], layout: 'hw'};

    let typedArray0 = new Float32Array(width * height);
    typedArray0.fill(meanValue * 1);
    preProcessInfo.getPreProcessChannel(0).meanData = {tensorDesc: tensorDesc, data: typedArray0.buffer};

    let typedArray1 = new Float32Array(width * height);
    typedArray1.fill(meanValue * 2);
    preProcessInfo.getPreProcessChannel(1).meanData = {tensorDesc: tensorDesc, data: typedArray1.buffer};

    let typedArray2 = new Float32Array(width * height);
    typedArray2.fill(meanValue * 3);
    preProcessInfo.getPreProcessChannel(2).meanData = {tensorDesc: tensorDesc, data: typedArray2.buffer};
    preProcessInfo.setVariant('mean_image');

    const execNet = await core.loadNetwork(net, 'CPU');

    const inferReq = await execNet.createInferRequest();
    const inputBlob = inferReq.getBlob(inputInfo.name());

    const inputData = new Float32Array(inputBlob.wmap());
    inputData.fill(0);

    inputBlob.unmap();

    inferReq.infer();

    const outputInfo = net.getOutputsInfo()[0];
    const outputBlob = inferReq.getBlob(outputInfo.name());
    const outputData = new Float32Array(outputBlob.rmap());

    outputBlob.unmap();

    for(let i=0; i<outputData.length; i++){
      if (i < 9){
        expect(outputData[i]).to.eql(inputData[i] - meanValue)
      } else if (i < 18){
        expect(outputData[i]).to.eql(inputData[i] - 2 * meanValue)
      } else {
        expect(outputData[i]).to.eql(inputData[i] - 3 * meanValue)
      }
    }
  });
});