const {binPathFromXML} = require("../../common");

class Engine {
    constructor(core, modelPath) {
        this.core = core;
        this.modelPath = modelPath;
    }

    async initialize(deviceName) {
        const weightsPath = binPathFromXML(this.modelPath);
        this.network = await this.core.readNetwork(this.modelPath, weightsPath);
        this.execNet = await this.core.loadNetwork(this.network, deviceName);

        this.inputsInfo = this.network.getInputsInfo();
        this.outputsInfo = this.network.getOutputsInfo();
    }

    async process(imagePath) {
    }
}

module.exports = {
    Engine
};