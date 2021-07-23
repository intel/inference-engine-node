const {binPathFromXML} = require("../../common");
const {Engine} = require("./engine");
const jimp = require('jimp');


class Result {
    constructor(data) {
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

    resizeROI(frameWidth, frameHeight) {
        this.position[0] *= frameWidth
        this.position[1] *= frameHeight
        this.size[0] = this.size[0] * frameWidth - this.position[0]
        this.size[1] = this.size[1] * frameHeight - this.position[1]
    }

    clip(width, height) {
        const min = [0, 0];
        const max = [width, height];
        this.position = this.position.map((value, index) => {
            if (value < min[index]) {
                return min[index]
            }
            if (value > max[index]) {
                return max[index]
            }
            return value
        })
        this.size = this.size.map((value, index) => {
            if (value < min[index]) {
                return min[index]
            }
            if (value > max[index]) {
                return max[index]
            }
            return value
        })
    }
}

function toCHWArray(image, blob) {
    const height = image.rows;
    const width = image.cols;
    const channels = image.channels;

    for (let h = 0; h < height; h++) {
        for (let w = 0; w < width; w++) {
            const pixel = image.at(h, w);
            const bgr = [pixel.x, pixel.y, pixel.z];
            for (let c = 0; c < channels; c++) {
                blob[c * width * height + h * height + w] = bgr[c];
            }
        }
    }

}

class FaceDetectionRetailEngine extends Engine {

    async initialize(deviceName) {
        const weightsPath = binPathFromXML(this.modelPath);
        this.network = await this.core.readNetwork(this.modelPath, weightsPath);
        this.inputsInfo = this.network.getInputsInfo();
        this.outputsInfo = this.network.getOutputsInfo();

        this.input = this.inputsInfo[0];
        this.input.setLayout('nchw');
        this.output = this.outputsInfo[0];
        this.execNet = await this.core.loadNetwork(this.network, deviceName);

    }

    async process(image) {
        const inputDimsFace = this.input.getDims();

        const inputHeight = inputDimsFace[2];
        const inputWidth = inputDimsFace[3];

        const frameWidth = image.cols;
        const frameHeight = image.rows;

        image = image.resize(inputHeight, inputWidth, 2);

        const inferenceRequest = this.execNet.createInferRequest();

        const inputBlob = inferenceRequest.getBlob(this.input.name());

        const inputData = new Float32Array(inputBlob.wmap());

        toCHWArray(image, inputData);

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