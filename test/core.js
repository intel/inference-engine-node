const describe = require('mocha').describe;
var chai = require('chai');
var chaiAsPromised = require('chai-as-promised');
chai.use(chaiAsPromised);
var expect = chai.expect;
var should = chai.should();

const model_path = './models/squeezenet1.1/FP16/squeezenet1.1.xml';
const weights_path = './models/squeezenet1.1/FP16/squeezenet1.1.bin';
const ie = require('../lib/ie');
const core = ie.createCore();

describe('Core Test', function() {
  it('getVersions should be a function', () => {
    expect(core.getVersions).to.be.a('function');
  });

  it('getVersions should return a map', () => {
    expect(core.getVersions('CPU')).to.be.an('map');
  });

  it('getVersions should throw for wrong number of argument', () => {
    expect(() => core.getVersions()).to.throw();
  });

  it('getVersions should throw for wrong type of argument', () => {
    expect(() => core.getVersions(1)).to.throw();
  });

  it('getVersions should throw for invalid argument', () => {
    expect(() => core.getVersions('foo')).to.throw();
  });

  it('PluginVersions should have "CPU" key', () => {
    expect(core.getVersions('CPU')).to.have.property('CPU');
  });

  it('Check the properties of Version of CPU plugin', () => {
    const version = core.getVersions('CPU')['CPU'];
    expect(version).to.be.a('object');
    expect(version).to.have.property('description');
    expect(version.description).to.be.a('string');
    expect(version).to.have.property('buildNumber');
    expect(version.buildNumber).to.be.a('string');
    expect(version).to.have.property('apiVersion');
    expect(version.apiVersion).to.be.a('object');
  });

  it('Check the properties of ApiVersion of CPU plugin', () => {
    const apiVersion = core.getVersions('CPU')['CPU'].apiVersion;
    expect(apiVersion).to.have.property('major');
    expect(apiVersion.major).to.be.a('number');
    expect(apiVersion).to.have.property('minor');
    expect(apiVersion.minor).to.be.a('number');
  });

  it('readNetwork should be a function', () => {
    expect(core.readNetwork).to.be.a('function');
  });

  it('readNetwork should return a Network object',
     async () => {
       expect(await core.readNetwork(model_path, weights_path))
           .to.be.a('Network');
     });

  it('readNetwork should throw for wrong number of argument',
     () => {
       expect(core.readNetwork()).to.be.rejectedWith(TypeError);
     });

  it('readNetwork should throw for wrong type of argument',
     () => {
       expect(core.readNetwork(1, 2)).to.be.rejectedWith(TypeError);
     });

  it('loadNetwork should be a function', () => {
    expect(core.loadNetwork).to.be.a('function');
  });

  it('loadNetwork should return an ExecutableNetwork', async () => {
    const net = await core.readNetwork(model_path, weights_path);
    expect(await core.loadNetwork(net, 'CPU')).to.be.a('ExecutableNetwork');
  });

  it('loadNetwork should reject for wrong number of argument', () => {
    expect(core.loadNetwork()).to.be.rejectedWith(TypeError);
  });

  it('loadNetwork should reject for wrong type of argument', async () => {
    const net = await core.readNetwork(model_path, weights_path);
    expect(core.loadNetwork(net, 2)).to.be.rejectedWith(TypeError);
  });

  it('loadNetwork should reject for 1st invalid argument', () => {
    const net = {};
    expect(core.loadNetwork(net, 'CPU')).to.be.rejectedWith(TypeError);
  });

  it('loadNetwork should reject for 2nd invalid argument', async () => {
    const net = await core.readNetwork(model_path, weights_path);
    expect(core.loadNetwork(net, 'foo')).to.be.rejectedWith(Error);
  });
});