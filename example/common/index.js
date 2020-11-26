const classification = require('./classification');
const objectDetection = require('./object_detection');


function showInputOutputInfo(info) {
    console.log(`  name: ${info.name()}`);
    console.log(`  precision: ${info.getPrecision()}`);
    console.log(`  layout: ${info.getLayout()}`);
    console.log(`  dims: [${info.getDims()}]`);
}

function showVersion(version) {
    console.log(
        `  API version: ${version.apiVersion.major}.${version.apiVersion.minor}`);
    console.log(`  Build: ${version.buildNumber}`);
    console.log(`  Description: ${version.description}`);
}

function showPluginVersions(versions) {
    Object.keys(versions).forEach(name => {
        console.log(`  Deivce Name: ${name}`);
        showVersion(versions[name]);
    });
}

function showBreakLine() {
    console.log('-------------------------------------------');
}

function highlight(msg) {
    console.log('\x1b[1m' + msg + '\x1b[0m');
}

function warning(msg) {
    console.log('\x1b[33m' + msg + '\x1b[0m');
}

function showAvailableDevices(core) {
    const devices = core.getAvailableDevices();
    console.log(`Available target devices: ${devices.join(' ')}`);
}

const xmlExtensionPattern = /\.xml$/g

function binPathFromXML(xmlPath) {
    return xmlPath.replace(xmlExtensionPattern, '.bin');
}

function labelsPathFromXML(xmlPath) {
    return xmlPath.replace(xmlExtensionPattern, '.labels');
}

module.exports = {
    binPathFromXML,
    classification,
    highlight,
    labelsPathFromXML,
    objectDetection,
    showAvailableDevices,
    showBreakLine,
    showInputOutputInfo,
    showVersion,
    showPluginVersions,
    warning
};
