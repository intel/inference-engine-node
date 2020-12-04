const describe = require('mocha').describe;
var chai = require('chai');
var chaiAsPromised = require('chai-as-promised');
chai.use(chaiAsPromised);
var expect = chai.expect;
var should = chai.should();

const ie = require('../lib/inference-engine-node');
const {it} = require('mocha');

describe('Network Test', function() {
  let network;
  before(async () => {
    const model_path = './models/squeezenet1.1/FP16/squeezenet1.1.xml';
    const weights_path = './models/squeezenet1.1/FP16/squeezenet1.1.bin';
    const core = new ie.Core();
    network = await core.readNetwork(model_path, weights_path);
  });

  it('getName should be a function', () => {
    expect(network.getName).to.be.a('function');
  });

  it('getName should return a string', () => {
    expect(network.getName()).to.be.a('string');
  });

  it('network name should be squeezenet1.1', () => {
    expect(network.getName()).to.equal('squeezenet1.1');
  });

  it('getName should throw for invalid argument', () => {
    expect(() => network.getName(1)).to.throw(TypeError);
  });

  it('getInputsInfo should be a function', () => {
    expect(network.getInputsInfo).to.be.a('function');
  });

  it('getInputsInfo should return an array', () => {
    expect(network.getInputsInfo()).to.be.a('array');
  });

  it('input info array length should be 1', () => {
    expect(network.getInputsInfo()).to.be.lengthOf(1);
  });

  it('input info array should contain InputInfo objects', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo).to.be.a('InputInfo');
  });

  it('InputInfo.name should be a function', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo.name).to.be.a('function');
  });

  it('InputInfo.name should return a string named "data"', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo.name()).to.be.a('string').equal('data');
  });

  it('InputInfo.name should throw for invalid argument', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(() => inputInfo.name(1)).to.throw(TypeError);
  });

  it('InputInfo.getPrecision should be a function', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo.getPrecision).to.be.a('function');
  });

  it('InputInfo.getPrecision should return a string named "fp32"', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo.getPrecision()).to.be.a('string').equal('fp32');
  });

  it('InputInfo.getPrecision should throw for invalid argument', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(() => inputInfo.getPrecision(1)).to.throw(TypeError);
  });

  it('InputInfo.setPrecision should be a function', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo.setPrecision).to.be.a('function');
  });

  it('InputInfo.setPrecision should set precision "u8"', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo.setPrecision('u8')).to.be.a('undefined');
    expect(inputInfo.getPrecision()).to.be.a('string').equal('u8');
  });

  it('InputInfo.setPrecision should throw for wrong type of argument', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(() => inputInfo.setPrecision(1)).to.throw(TypeError);
  });

  it('InputInfo.setPrecision should throw for invalid argument', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(() => inputInfo.setPrecision('foo')).to.throw(TypeError);
  });

  it('InputInfo.getLayout should be a function', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo.getLayout).to.be.a('function');
  });

  it('InputInfo.getLayout should return a string named "nchw"', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo.getLayout()).to.be.a('string').equal('nchw');
  });

  it('InputInfo.getLayout should throw for invalid argument', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(() => inputInfo.getLayout(1)).to.throw(TypeError);
  });

  it('InputInfo.setLayout should be a function', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo.setLayout).to.be.a('function');
  });

  it('InputInfo.setLayout should set precision "nhwc"', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo.setLayout('nhwc')).to.be.a('undefined');
    expect(inputInfo.getLayout()).to.be.a('string').equal('nhwc');
  });

  it('InputInfo.setLayout should throw for wrong type of argument', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(() => inputInfo.setLayout(1)).to.throw(TypeError);
  });

  it('InputInfo.setLayout should throw for invalid argument', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(() => inputInfo.setLayout('foo')).to.throw(TypeError);
  });

  it('InputInfo.getDims should be a function', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo.getDims).to.be.a('function');
  });

  it('InputInfo.getDims should return an array of length 4', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo.getDims()).to.be.a('array').with.lengthOf(4);
  });

  it('InputInfo.getDims should return an array with value [1, 3, 227, 227]',
     () => {
       const inputInfo = network.getInputsInfo()[0];
       expect(inputInfo.getDims()).to.be.a('array').to.deep.equal([
         1, 3, 227, 227
       ]);
     });

  it('InputInfo.getDims should throw for invalid argument', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(() => inputInfo.getDims(1)).to.throw(TypeError);
  });

  it('InputInfo.getPreProcess should be a function', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo.getPreProcess).to.be.a('function');
  });

  it('InputInfo.getPreProcess should return a preprocessInfo object', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(inputInfo.getPreProcess()).to.be.a('preprocessInfo');
  });

  it('InputInfo.getPreProcess should throw for invalid argument', () => {
    const inputInfo = network.getInputsInfo()[0];
    expect(() => inputInfo.getPreProcess(1)).to.throw(TypeError);
  });

  it('PreProcessInfo.getColorFormat should be a function', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.getColorFormat).to.be.a('function');
  });

  it('PreProcessInfo.getColorFormat should return a string', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.getColorFormat()).to.be.a('string');
  });

  it('PreProcessInfo.getColorFormat should throw for invalid argument', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(() => preprocessInfo.getColorFormat(1)).to.throw(TypeError);
  });

  it('PreProcessInfo.setColorFormat should be a function', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.setColorFormat).to.be.a('function');
  });

  it('PreProcessInfo.setColorFormat should set colorformat "bgr"', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.setColorFormat('bgr')).to.be.a('undefined');
    expect(preprocessInfo.getColorFormat()).to.be.a('string').equal('bgr');
  });

  it('PreProcessInfo.setColorFormat should throw for wrong type of argument',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       expect(() => preprocessInfo.setColorFormat(1)).to.throw(TypeError);
     });

  it('PreProcessInfo.setColorFormat should throw for invalid argument', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(() => preprocessInfo.setColorFormat('foo')).to.throw(TypeError);
  });

  it('PreProcessInfo.getResizeAlgorithm should be a function', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.getResizeAlgorithm).to.be.a('function');
  });

  it('PreProcessInfo.getResizeAlgorithm should return a string', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.getResizeAlgorithm()).to.be.a('string');
  });

  it('PreProcessInfo.getResizeAlgorithm should throw for invalid argument',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       expect(() => preprocessInfo.getResizeAlgorithm(1)).to.throw(TypeError);
     });

  it('PreProcessInfo.setResizeAlgorithm should be a function', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.setResizeAlgorithm).to.be.a('function');
  });

  it('PreProcessInfo.setResizeAlgorithm should set algorithm "resize_bilinear"',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       expect(preprocessInfo.setResizeAlgorithm('resize_bilinear'))
           .to.be.a('undefined');
       expect(preprocessInfo.getResizeAlgorithm())
           .to.be.a('string')
           .equal('resize_bilinear');
     });

  it('PreProcessInfo.setResizeAlgorithm should throw for wrong type of argument',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       expect(() => preprocessInfo.setResizeAlgorithm(1)).to.throw(TypeError);
     });

  it('PreProcessInfo.setResizeAlgorithm should throw for invalid argument',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       expect(() => preprocessInfo.setResizeAlgorithm('foo'))
           .to.throw(TypeError);
     });

  it('PreProcessInfo.getMeanVariant should be a function', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.getMeanVariant).to.be.a('function');
  });

  it('PreProcessInfo.getMeanVariant should return a string', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.getMeanVariant()).to.be.a('string');
  });

  it('PreProcessInfo.getMeanVariant should throw for invalid argument', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(() => preprocessInfo.getMeanVariant(1)).to.throw(TypeError);
  });

  it('PreProcessInfo.setVariant should be a function', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.setVariant).to.be.a('function');
  });

  it('PreProcessInfo.setVariant should set type of mean "mean_value"', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.setVariant('mean_value')).to.be.a('undefined');
    expect(preprocessInfo.getMeanVariant())
        .to.be.a('string')
        .equal('mean_value');
  });

  it('PreProcessInfo.setVariant should throw for wrong type of argument',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       expect(() => preprocessInfo.setVariant(1)).to.throw(TypeError);
     });

  it('PreProcessInfo.setVariant should throw for invalid argument', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(() => preprocessInfo.setVariant('foo')).to.throw(TypeError);
  });

  it('PreProcessInfo.init() should be a function', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.init).to.be.a('function');
  })

  it('PreProcessInfo.init should be a function', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.init).to.be.a('function');
  })

  it('PreProcessInfo.init should throw for wrong number of arguments', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(() => preprocessInfo.init(1, 3)).to.throw(TypeError);
  });

  it('PreProcessInfo.init should throw for invalid argument', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(() => preprocessInfo.init('foo')).to.throw(TypeError);
  });

  it('PreProcessInfo.getNumberOfChannels should be a function', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.getNumberOfChannels).to.be.a('function');
  });

  it('PreProcessInfo.getNumberOfChannels should return a number', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.getNumberOfChannels()).to.be.a('number');
  });

  it('PreProcessInfo.getNumberOfChannels should throw for invalid argument',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       expect(() => preprocessInfo.getNumberOfChannels(1)).to.throw(TypeError);
     });

  it('PreProcessInfo.getPreProcessChannel should be a function', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.getPreProcessChannel).to.be.a('function');
  });

  it('PreProcessChannel should throw when the number of channels is 0', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.getNumberOfChannels()).to.be.a('number').equal(0);
    expect(() => preprocessInfo.getPreProcessChannel(0)).to.throw();
  });

  it('Check the properties of PreProcessChannel', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    preprocessInfo.init(3);
    const perProcessChannel = preprocessInfo.getPreProcessChannel(0);
    expect(perProcessChannel).to.be.a('object');
    expect(perProcessChannel).to.have.property('stdScale');
    expect(perProcessChannel.stdScale).to.be.a('number');
    expect(perProcessChannel).to.have.property('meanValue');
    expect(perProcessChannel.meanValue).to.be.a('number');
    expect(perProcessChannel).to.have.property('meanData');
    expect(perProcessChannel.meanData).to.be.a('null');
  });

  it('PreProcessInfo.getPreProcessChannel should throw for wrong type of argument',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       expect(() => preprocessInfo.getPreProcessChannel('foo'))
           .to.throw(TypeError);
     });

  it('PreProcessInfo.getPreProcessChannel should throw for wrong numbers of argument',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       expect(() => preprocessInfo.getPreProcessChannel(1, 2))
           .to.throw(TypeError);
     });

  it('PreProcessInfo.setPreProcessChannel should be a function', () => {
    const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
    expect(preprocessInfo.setPreProcessChannel).to.be.a('function');
  });

  it('PreProcessInfo.setPreProcessChannel should set the stdScale and the meanValue',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       let width = 32;
       let height = 32;
       let typedArray1 = new Float32Array(width * height);
       typedArray1.fill(32.0);
       let tensorDesc = {
         precision: 'fp32',
         dims: [width, height],
         layout: 'hw'
       };
       let meanData = {desc: tensorDesc, data: typedArray1.buffer};
       preprocessInfo.setPreProcessChannel(
           0, {'stdScale': 127.5, 'meanValue': 127.5, 'meanData': meanData});
       const preProcessChannel = preprocessInfo.getPreProcessChannel(0);
       expect(preProcessChannel.meanValue).to.be.a('number').equal(127.5);
       expect(preProcessChannel.stdScale).to.be.a('number').equal(127.5);
       expect(new Float32Array(preProcessChannel.meanData)[0]).equal(32.0);
     });

  it('PreProcessInfo.setPreProcessChannel should should throw for wrong number of arguments',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       const preProcessChannel = preprocessInfo.getPreProcessChannel(0);
       console.log(new Float32Array(preProcessChannel.meanData)[0])

       expect(() => preprocessInfo.setPreProcessChannel(1)).to.throw(TypeError);
     });

  it('PreProcessInfo.setPreProcessChannel should should throw for wrong number of arguments',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       expect(
           () => preprocessInfo.setPreProcessChannel(
               0, {'stdScale': 127.5, 'meanValue': 127.5}, 1))
           .to.throw(TypeError);
     });

  it('PreProcessInfo.setPreProcessChannel should should throw for wrong type of arguments',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       expect(() => preprocessInfo.setPreProcessChannel('foo', {
         'stdScale': 127.5,
         'meanValue': 127.5
       })).to.throw(TypeError);
     });

  it('PreProcessInfo.setPreProcessChannel should should throw for wrong type of arguments',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       expect(() => preprocessInfo.setPreProcessChannel('foo', 'foo2'))
           .to.throw(TypeError);
     });

  it('PreProcessInfo.setPreProcessChannel should should throw for wrong type of stdScale and meanValue',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       expect(() => preprocessInfo.setPreProcessChannel(0, 0, {
         'stdScale': 'foo',
         'meanValue': 127.5
       })).to.throw(TypeError);
       expect(() => preprocessInfo.setPreProcessChannel(0, 0, {
         'stdScale': 127.5,
         'meanValue': 'foo'
       })).to.throw(TypeError);
     });

  it('PreProcessInfo.setPreProcessChannel should should throw for missing precision, dims, or layout',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       let width = 32;
       let height = 32;
       let typedArray1 = new Float32Array(width * height);
       expect(() => preprocessInfo.setPreProcessChannel(0, 0, {
         'stdScale': 127.5,
         'meanValue': 127.5,
         'meanData': {
           desc: {dims: [width, height], layout: 'hw'},
           data: typedArray1.buffer
         }
       })).to.throw(TypeError);
       expect(() => preprocessInfo.setPreProcessChannel(0, 0, {
         'stdScale': 127.5,
         'meanValue': 127.5,
         'meanData':
             {desc: {precision: 'fp32', layout: 'hw'}, data: typedArray1.buffer}
       })).to.throw(TypeError);
       expect(() => preprocessInfo.setPreProcessChannel(0, 0, {
         'stdScale': 127.5,
         'meanValue': 127.5,
         'meanData': {
           desc: {precision: 'fp32', dims: [width, height]},
           data: typedArray1.buffer
         }
       })).to.throw(TypeError);
     });

  it('PreProcessInfo.setPreProcessChannel should should throw for wrong type of precision, dims, and layout',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       let width = 32;
       let height = 32;
       let typedArray1 = new Float32Array(width * height);
       expect(() => preprocessInfo.setPreProcessChannel(0, 0, {
         'stdScale': 127.5,
         'meanValue': 127.5,
         'meanData': {
           desc: {precision: 1, dims: [width, height], layout: 'hw'},
           data: typedArray1.buffer
         }
       })).to.throw(TypeError);
       expect(() => preprocessInfo.setPreProcessChannel(0, 0, {
         'stdScale': 127.5,
         'meanValue': 127.5,
         'meanData': {
           desc: {precision: 'fp32', dims: 1, layout: 'hw'},
           data: typedArray1.buffer
         }
       })).to.throw(TypeError);
       expect(() => preprocessInfo.setPreProcessChannel(0, 0, {
         'stdScale': 127.5,
         'meanValue': 127.5,
         'meanData': {
           desc: {precision: 'fp32', dims: [width, height], layout: 1},
           data: typedArray1.buffer
         }
       })).to.throw(TypeError);
     });

  it('PreProcessInfo.setPreProcessChannel should should throw for wrong type of data',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       let width = 32;
       let height = 32;
       let typedArray1 = new Float32Array(width * height);
       expect(() => preprocessInfo.setPreProcessChannel(0, 0, {
         'stdScale': 127.5,
         'meanValue': 127.5,
         'meanData': {
           desc: {precision: 'fp32', dims: [width, height], layout: 'hw'},
           data: typedArray1
         }
       })).to.throw(TypeError);
     });

  it('PreProcessInfo.setPreProcessChannel should should throw for unmatched data and dims',
     () => {
       const preprocessInfo = network.getInputsInfo()[0].getPreProcess();
       let width = 32;
       let height = 32;
       let typedArray1 = new Float32Array(width * height);
       expect(() => preprocessInfo.setPreProcessChannel(0, 0, {
         'stdScale': 127.5,
         'meanValue': 127.5,
         'meanData': {
           desc: {precision: 'fp32', dims: [16, height], layout: 'hw'},
           data: typedArray1.buffer
         }
       })).to.throw(TypeError);
     });

  it('getOutputsInfo should be a function', () => {
    expect(network.getOutputsInfo).to.be.a('function');
  });

  it('getOutputsInfo should return an array', () => {
    expect(network.getOutputsInfo()).to.be.a('array');
  });

  it('output info array length should be 1', () => {
    expect(network.getOutputsInfo()).to.be.lengthOf(1);
  });

  it('output info array should contain outputInfo objects', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(outputInfo).to.be.a('outputInfo');
  });

  it('outputInfo.name should be a function', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(outputInfo.name).to.be.a('function');
  });

  it('outputInfo.name should return a string named "prob"', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(outputInfo.name()).to.be.a('string').equal('prob');
  });

  it('outputInfo.name should throw for invalid argument', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(() => outputInfo.name(1)).to.throw(TypeError);
  });

  it('outputInfo.getPrecision should be a function', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(outputInfo.getPrecision).to.be.a('function');
  });

  it('outputInfo.getPrecision should return a string named "fp32"', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(outputInfo.getPrecision()).to.be.a('string').equal('fp32');
  });

  it('outputInfo.getPrecision should throw for invalid argument', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(() => outputInfo.getPrecision(1)).to.throw(TypeError);
  });

  it('outputInfo.setPrecision should be a function', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(outputInfo.setPrecision).to.be.a('function');
  });

  it('outputInfo.setPrecision should set precision "fp16"', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(outputInfo.setPrecision('fp16')).to.be.a('undefined');
    expect(outputInfo.getPrecision()).to.be.a('string').equal('fp16');
  });

  it('outputInfo.setPrecision should throw for wrong type of argument', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(() => outputInfo.setPrecision(1)).to.throw(TypeError);
  });

  it('outputInfo.setPrecision should throw for invalid argument', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(() => outputInfo.setPrecision('foo')).to.throw(TypeError);
  });

  it('outputInfo.getLayout should be a function', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(outputInfo.getLayout).to.be.a('function');
  });

  it('outputInfo.getLayout should return a string named "nchw"', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(outputInfo.getLayout()).to.be.a('string').equal('nchw');
  });

  it('outputInfo.getLayout should throw for invalid argument', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(() => outputInfo.getLayout(1)).to.throw(TypeError);
  });

  it('outputInfo.getDims should be a function', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(outputInfo.getDims).to.be.a('function');
  });

  it('outputInfo.getDims should return an array of length 4', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(outputInfo.getDims()).to.be.a('array').with.lengthOf(4);
  });

  it('outputInfo.getDims should return an array with value [1, 1000, 1, 1]',
     () => {
       const outputInfo = network.getOutputsInfo()[0];
       expect(outputInfo.getDims()).to.be.a('array').to.deep.equal([
         1, 1000, 1, 1
       ]);
     });

  it('outputInfo.getDims should throw for invalid argument', () => {
    const outputInfo = network.getOutputsInfo()[0];
    expect(() => outputInfo.getDims(1)).to.throw(TypeError);
  });
});