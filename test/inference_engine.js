const describe = require('mocha').describe;
var chai = require('chai');
var chaiAsPromised = require('chai-as-promised');
chai.use(chaiAsPromised);
var expect = chai.expect;
var should = chai.should();

const ie = require('../lib/ie');
const model_path = './models/squeezenet1.1/FP16/squeezenet1.1.xml';
const weights_path = './models/squeezenet1.1/FP16/squeezenet1.1.bin';

describe('InferenceEngine Test', () => {
  it('InferenceEngine should be exported', () => {
    should.exist(require('../lib/ie'));
  });

  it('InferenceEngine.getVersion should be a fnction', () => {
    expect(ie.getVersion).to.be.a('function');
  });

  it('InferenceEngine.getVersion should return an object', () => {
    expect(ie.getVersion()).to.be.a('object');
  });

  it('InferenceEngine.getVersion should throw for invalid argument', () => {
    expect(() => ie.getVersion('foo')).to.throw();
  });

  it('Check the properties of Version object', () => {
    const version = ie.getVersion();
    expect(version).to.be.a('object');
    expect(version).to.have.property('description');
    expect(version.description).to.be.a('string');
    expect(version).to.have.property('buildNumber');
    expect(version.buildNumber).to.be.a('string');
    expect(version).to.have.property('apiVersion');
    expect(version.apiVersion).to.be.a('object');
  });

  it('Check the properties of ApiVersion object', () => {
    const apiVersion = ie.getVersion().apiVersion;
    expect(apiVersion).to.have.property('major');
    expect(apiVersion.major).to.be.a('number');
    expect(apiVersion).to.have.property('minor');
    expect(apiVersion.minor).to.be.a('number');
  });

  it('InferenceEngine.createCore should be a function', () => {
    expect(ie.createCore).to.be.a('function');
  });

  it('InferenceEngine.createCore should return a Core object', () => {
    expect(ie.createCore()).to.be.a('Core');
  });

  it('InferenceEngine.createCore should throw for invalid argument', () => {
    expect(() => ie.createCore('foo')).to.throw();
  });
});