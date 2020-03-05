const describe = require('mocha').describe;
var chai = require('chai');
var chaiAsPromised = require('chai-as-promised');
chai.use(chaiAsPromised);
var expect = chai.expect;
var assert = chai.assert;

const ie = require('../lib/ie');

// Reference results from C++ sample
const output_references = [0.000021, 0.000089, 0.000050, 0.000424, 0.006300];
function almostEqual(a, b) {
  return Math.abs(a - b) < 1e-6;
}

describe('ExecutableNetwork Test', function() {
  let exec_net;
  before(async () => {
    const model_path = './models/squeezenet1.1/FP16/squeezenet1.1.xml';
    const weights_path = './models/squeezenet1.1/FP16/squeezenet1.1.bin';
    const core = ie.createCore();
    const net = await core.readNetwork(model_path, weights_path);
    exec_net = await core.loadNetwork(net, 'CPU');
  });

  it('ExecutableNetwork.createInferRequest should be a function', () => {
    expect(exec_net.createInferRequest).to.be.a('function');
  });

  it('ExecutableNetwork.createInferRequest should return a InferRequest',
     () => {
       expect(exec_net.createInferRequest()).to.be.a('InferRequest');
     });

  it('ExecutableNetwork.createInferRequest should throw for invalid argument',
     () => {
       expect(() => exec_net.createInferRequest('foo')).to.throw(TypeError);
     });

  it('InferRequest.getBlob should be a function', () => {
    const infer_req = exec_net.createInferRequest();
    expect(infer_req.getBlob).to.be.a('function');
  });

  it('InferRequest.getBlob should return input Blob', () => {
    const infer_req = exec_net.createInferRequest();
    expect(infer_req.getBlob('data')).to.be.a('Blob');
  });

  it('InferRequest.getBlob should return output Blob', () => {
    const infer_req = exec_net.createInferRequest();
    expect(infer_req.getBlob('prob')).to.be.a('Blob');
  });

  it('InferRequest.getBlob should throw for wrong number of arguments', () => {
    const infer_req = exec_net.createInferRequest();
    expect(() => infer_req.getBlob()).to.throw(TypeError);
  });

  it('InferRequest.getBlob should throw for wrong type of arguments', () => {
    const infer_req = exec_net.createInferRequest();
    expect(() => infer_req.getBlob(1)).to.throw(TypeError);
  });

  it('InferRequest.getBlob should throw for invalid arguments', () => {
    const infer_req = exec_net.createInferRequest();
    expect(() => infer_req.getBlob('foo')).to.throw(RangeError);
  });

  it('Blob.buffer should be a function', () => {
    const blob = exec_net.createInferRequest().getBlob('data');
    expect(blob.buffer).to.be.a('function');
  });

  it('Blob.buffer should return an ArrayBuffer with byteLength 1*3*227*227*4',
     () => {
       const blob = exec_net.createInferRequest().getBlob('data');
       expect(blob.buffer()).to.be.a('arraybuffer');
       expect(blob.buffer().byteLength).to.equal(1 * 3 * 227 * 227 * 4);
     });

  it('Blob.buffer should throw for invalid argument', () => {
    const blob = exec_net.createInferRequest().getBlob('data');
    expect(() => blob.buffer(1)).to.throw(TypeError);
  });

  it('Blob.size should be a function', () => {
    const blob = exec_net.createInferRequest().getBlob('data');
    expect(blob.size).to.be.a('function');
  });

  it('Blob.size should return a number with value that equals 1*3*227*227',
     () => {
       const blob = exec_net.createInferRequest().getBlob('data');
       expect(blob.size()).to.be.a('number').to.equal(1 * 3 * 227 * 227);
     });

  it('Blob.size should throw for invalid argument', () => {
    const blob = exec_net.createInferRequest().getBlob('data');
    expect(() => blob.size(1)).to.throw(TypeError);
  });

  it('Blob.byteSize should be a function', () => {
    const blob = exec_net.createInferRequest().getBlob('data');
    expect(blob.byteSize).to.be.a('function');
  });

  it('Blob.byteSize should return a number with value that equals 1*3*227*227*4',
     () => {
       const blob = exec_net.createInferRequest().getBlob('data');
       expect(blob.byteSize())
           .to.be.a('number')
           .to.equal(1 * 3 * 227 * 227 * 4);
     });

  it('Blob.byteSize should throw for invalid argument', () => {
    const blob = exec_net.createInferRequest().getBlob('data');
    expect(() => blob.byteSize(1)).to.throw(TypeError);
  });

  it('InferRequest.infer should be a function', () => {
    const infer_req = exec_net.createInferRequest();
    expect(infer_req.infer).to.be.a('function');
  });

  it('InferRequest.infer should return null', () => {
    const infer_req = exec_net.createInferRequest();
    expect(infer_req.infer()).to.be.a('null');
  });

  it('InferRequest.infer should throw for invalid argument', () => {
    const infer_req = exec_net.createInferRequest();
    expect(() => infer_req.infer('foo')).to.throw(TypeError);
  });

  it('Check InferRequest.infer result', () => {
    const infer_req = exec_net.createInferRequest();
    const input_blob = infer_req.getBlob('data');
    const input_data = new Float32Array(input_blob.buffer());
    for (let i = 0; i < input_blob.size(); i++) {
      input_data[i] = i / (input_blob.size() + 1);
    }
    infer_req.infer();
    const output_blob = infer_req.getBlob('prob');
    const output_data = new Float32Array(output_blob.buffer());
    for (let i = 0; i < output_references.length; i++) {
      assert(
          almostEqual(output_data[i], output_references[i]),
          `output data equals to reference data`);
    }
  });

  it('InferRequest.startAsync should be a function', () => {
    const infer_req = exec_net.createInferRequest();
    expect(infer_req.startAsync).to.be.a('function');
  });

  it('InferRequest.startAsync should return a promise', () => {
    const infer_req = exec_net.createInferRequest();
    expect(infer_req.startAsync()).to.be.a('promise');
  });

  it('InferRequest.startAsync should reject for invalid argument', () => {
    const infer_req = exec_net.createInferRequest();
    expect(infer_req.startAsync('foo')).to.be.rejectedWith(TypeError);
    ;
  });

  it('Check InferRequest.startAsync result', async () => {
    const infer_req = exec_net.createInferRequest();
    const input_blob = infer_req.getBlob('data');
    const input_data = new Float32Array(input_blob.buffer());
    for (let i = 0; i < input_blob.size(); i++) {
      input_data[i] = i / (input_blob.size() + 1);
    }
    await infer_req.startAsync();
    const output_blob = infer_req.getBlob('prob');
    const output_data = new Float32Array(output_blob.buffer());
    for (let i = 0; i < output_references.length; i++) {
      assert(
          almostEqual(output_data[i], output_references[i]),
          `output data equals to reference data`);
    }
  });
});