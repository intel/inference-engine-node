const describe = require('mocha').describe;
const expect = require('chai').expect;
const ie = require('../lib/ie');

const model_path = './models/squeezenet1.1/FP16/squeezenet1.1.xml';
const weights_path = './models/squeezenet1.1/FP16/squeezenet1.1.bin';

describe('InferenceEngine Test', function() {
  it('InferenceEngine should be exported', function() {
    expect(ie).to.be.a('object');
  });

  it('InferenceEngine.getVersion should be a fnction', function() {
    expect(ie.getVersion).to.be.a('function');
  });

  it('InferenceEngine.getVersion should return an object', function() {
    expect(ie.getVersion()).to.be.a('object');
  });

  it('InferenceEngine.getVersion should throw for invalid argument', function() {
    expect(() => ie.getVersion('foo')).to.throw();
  });

  it('Check the properties of Version object', function() {
    const version = ie.getVersion();
    expect(typeof version === 'object');
    expect(version.hasOwnProperty('description'));
    expect(typeof version.description === 'string');
    expect(version.hasOwnProperty('buildNumber'));
    expect(typeof version.buildNumber === 'string');
    expect(version.hasOwnProperty('apiVersion'));
    expect(typeof version.apiVersion === 'object');
  });

  it('Check the properties of ApiVersion object', function() {
    const version = ie.getVersion();
    expect(version.hasOwnProperty('apiVersion'));
    expect(typeof version.apiVersion === 'object');
    const apiVersion = version.apiVersion;
    expect(apiVersion.hasOwnProperty('major'));
    expect(typeof apiVersion.major === 'number');
    expect(apiVersion.hasOwnProperty('minor'));
    expect(typeof apiVersion.minor === 'number');
  });

  it('InferenceEngine.createCore should be a function', function() {
    expect(typeof ie.createCore === 'function');
  });

  it('InferenceEngine.createCore should return an object', function() {
    expect(typeof ie.createCore() === 'object');
  });

  it('InferenceEngine.createCore should throw for invalid argument', function() {
    expect(() => ie.createCore('foo')).to.throw();
  });

  it('InferenceEngine.createNetwork should be a function', function() {
    expect(typeof ie.createNetwork === 'function');
  });

  it('InferenceEngine.createNetwork should return an object', async function() {
    const network = await ie.createNetwork(model_path, weights_path);
    expect(typeof network === 'object');
  });

  it('InferenceEngine.createNetwork should throw for wrong number of argument', async function() {
    expect(() => ie.createNetwork()).to.be.rejected;
  });

  it('InferenceEngine.createNetwork should throw for wrong type of argument', async function() {
    expect(await ie.createNetwork(1, 2)).to.throw(TypeError);
  });
});