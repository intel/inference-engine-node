const {binPathFromXML} = require("../../common");
const {Engine} = require("./engine");
const jimp = require('jimp');


class Result {
    constructor(data) {
        this.imageId = data[0]
        this.label = data[1]
        this.confidence = data[2]
        this.position = [data[3], data[4]]
        this.size = [data[5], data[6]]
    }

    rescaleROI(roiScaleFactor = 1.15) {
        this.position = this.position.map((value, index) => {
            return value - this.size[index] * 0.5 * (roiScaleFactor - 1);
        });
        this.size = this.size.map(value => value * roiScaleFactor);
    }

    resizeROI(frameWidth, frameHeight){
        this.position[0] *= frameWidth
        this.position[1] *= frameHeight
        this.size[0] = this.size[0]* frameWidth - this.position[0]
        this.size[1] = this.size[1]*frameHeight - this.position[1]
    }

    clip(width, height){
        const min = [0, 0];
        const max = [width, height];
        this.position = this.position.map((value, index) =>{
            if (value < min[index]){
                return min[index]
            }
            if (value > max[index]){
                return max[index]
            }
            return value
        })
        this.size = this.size.map((value, index) =>{
            if (value < min[index]){
                return min[index]
            }
            if (value > max[index]){
                return max[index]
            }
            return value
        })
    }
}

class FaceDetectionRetailEngine extends Engine {

    async initialize(deviceName) {
        const weightsPath = binPathFromXML(this.modelPath);
        this.network = await this.core.readNetwork(this.modelPath, weightsPath);

        this.inputsInfo = this.network.getInputsInfo();
        this.outputsInfo = this.network.getOutputsInfo();

        this.input = this.inputsInfo[0];
        this.input.setLayout('nhwc');
        this.output = this.outputsInfo[0];
        this.execNet = await this.core.loadNetwork(this.network, deviceName);

    }

    async process(image) {
        const inputDimsFace = this.input.getDims();

        const inputHeight = inputDimsFace[2];
        const inputWidth = inputDimsFace[3];
        const frameWidth = image.bitmap.width;
        const frameHeight = image.bitmap.height;

        if (image.bitmap.height !== inputHeight ||
            image.bitmap.width !== inputWidth) {
            console.log(`Resize image from (${image.bitmap.height}, ${
                image.bitmap.width}) to (${inputHeight}, ${inputWidth}).`);
            image.resize(inputWidth, inputHeight, jimp.RESIZE_BILINEAR);
        }

        const inferenceRequest = this.execNet.createInferRequest();

        const inputBlob = inferenceRequest.getBlob(this.input.name());

        const inputData = new Float32Array(inputBlob.wmap());

        image.scan(
            0, 0, image.bitmap.width, image.bitmap.height, function (x, y, idx) {
                // Convert from RGBA to BGR (IE default)
                let i = Math.floor(idx / 4) * 3;
                inputData[i + 2] = image.bitmap.data[idx + 0];  // R
                inputData[i + 1] = image.bitmap.data[idx + 1];  // G
                inputData[i + 0] = image.bitmap.data[idx + 2];  // B
            });

        inputBlob.unmap();

        inferenceRequest.infer();

        const inferenceResult = inferenceRequest.getBlob(this.output.name());
        const inferenceResultArray = new Float32Array(inferenceResult.rmap());

        const faces = [];
        for (let i = 0; i < inferenceResultArray.length; i += 7) {

            const confidence = inferenceResultArray[i + 2];

            if (confidence < 0.5) {
                continue;
            }

            const faceData = inferenceResultArray.slice(i, i + 7)
            const face = new Result(faceData)

            face.resizeROI(frameWidth, frameHeight);
            face.rescaleROI();
            faces.push(face);
        }

        inferenceResult.unmap();

        return faces;
    }
}

module.exports = {
    FaceDetectionRetailEngine: FaceDetectionRetailEngine
};