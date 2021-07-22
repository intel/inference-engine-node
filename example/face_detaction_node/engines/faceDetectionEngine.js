const {binPathFromXML} = require("../../common");
const {Engine} = require("./engine");
const jimp = require('jimp');


class FaceDetectionEngine extends Engine {

    async initialize(deviceName) {
        const weightsPath = binPathFromXML(this.modelPath);
        this.network = await this.core.readNetwork(this.modelPath, weightsPath);
        this.execNet = await this.core.loadNetwork(this.network, deviceName);

        this.inputsInfo = this.network.getInputsInfo();
        this.outputsInfo = this.network.getOutputsInfo();

        this.input = this.inputsInfo[0];
        this.input.setLayout('nhwc');
        this.input.setPrecision('u8');
        this.output = this.outputsInfo[0];
    }

    async process(image) {

        const inputDimsFace = this.input.getDims();

        const inputHeight = inputDimsFace[2];
        const inputWidth = inputDimsFace[3];

        // image.resize(inputHeight, inputWidth)
        if (image.bitmap.height !== inputHeight && image.bitmap.width !== inputWidth) {
            console.log(`Resize the image from ${image.bitmap.width}x${image.bitmap.height} to ${inputWidth}x${inputHeight}`)
            image.resize(inputWidth, inputHeight, jimp.RESIZE_BILINEAR);
        }

        const inferenceRequest = this.execNet.createInferRequest();

        const inputBlob = inferenceRequest.getBlob(this.input.name());
        console.log(this.input.name());
        const inputData = new Uint8Array(inputBlob.wmap());

        image.scan(
            0, 0, inputHeight, inputWidth,
            (x, y, idx) => {
                // Convert from RGBA to BGR (IE default)
                let i = Math.floor(idx / 4) * 3;
                inputData[i + 2] = image.bitmap.data[idx + 0];  // R
                inputData[i + 1] = image.bitmap.data[idx + 1];  // G
                inputData[i + 0] = image.bitmap.data[idx + 2];  // B
            }
        );

        inputBlob.unmap();

        inferenceRequest.infer();

        const inferenceResult = inferenceRequest.getBlob("boxes");
        const inferenceResultArray = new Float32Array(inferenceResult.rmap());

        const faces = [];
        for (let i = 0; i < inferenceResultArray.length; i += 5) {
            const confidence = inferenceResultArray[i + 4]
            if (confidence > 0.5) {
                faces.push(inferenceResultArray.slice(i, i + 5));
            }
        }

        inferenceResult.unmap();
        return faces;


    }
}

module.exports = {
    FaceDetectionEngine: FaceDetectionEngine
};