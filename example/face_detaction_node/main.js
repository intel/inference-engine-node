const {FaceDetectionRetailEngine} = require("./engines/faceDetectionRetailEngine");

const {Core, getVersion} = require('../../lib/inference-engine-node');
const cv = require('opencv4nodejs')


const option_definitions = [
    {
        name: 'help',
        alias: 'h',
        type: Boolean,
        description: 'Show this help message and exit.'
    },
    {
        name: 'image',
        alias: 'i',
        type: String,
        description: 'Required. Path to an image file.'
    },

    {
        name: 'face_detection_model',
        type: String,
        description: 'Required. Path to an .xml file with a trained model. Supported models: face-detection-retail-*'
    },
    {
        name: 'output',
        alias: 'o',
        type: String,
        defaultValue: 'out.jpg',
        description: 'Optional. Path to an output image file.'
    },
    {
        name: 'face_detection_device',
        type: String,
        defaultValue: 'CPU',
        description: 'Optional. Specify the target device to infer on ' +
            '(the list of available devices is shown below). ' +
            'Default value is CPU.'
    },
    {
        name: 'threshold',
        alias: 't',
        type: Number,
        defaultValue: 0.5,
        description:
            'Optional. The minimal probability of the visibility face. Default value is 0.5'
    }
];

const commandLineArgs = require('command-line-args');
const commandLineUsage = require('command-line-usage');
const {showVersion} = require("../common");
const {showBreakLine} = require("../common");


async function main() {

    const options = commandLineArgs(option_definitions);

    if (options.help || !options.image || !options.face_detection_model) {
        const usage = commandLineUsage([
            {
                header: 'Face Detection',
                content:
                    'An example of Face Detection using inference-engine-node.'
            },
            {header: 'Options', optionList: option_definitions}
        ]);
        console.log(usage);
        process.exit(0);
    }

    console.log('Start.')
    showBreakLine();

    console.log(`Check inference engine version: `);
    showVersion(getVersion());
    showBreakLine();

    const core = new Core();
    const color = new cv.Vec3(12.5, 255, 255);
    const faceEngine = new FaceDetectionRetailEngine(core, options.face_detection_model);
    await faceEngine.initialize(options.face_detection_device);
    cv.imreadAsync(options.image).then((image) => {
            console.log('Detected Faces:');

            console.log('probability'.padEnd(15) + 'box');
            const header = '-------';
            console.log(header.padEnd(15) + header);

            faceEngine.process(image, options.threshold).then(detectedFace =>
                detectedFace.forEach((face) => {
                        const xMin = face.position[0].toFixed(2);
                        const yMin = face.position[1].toFixed(2);
                        const width = face.size[0].toFixed(2);
                        const height = face.size[1].toFixed(2);
                        const confidence = face.confidence.toFixed(2).toString();

                        console.log(confidence.padEnd(15)
                            + `[${xMin},${yMin},${width},${height}]`);

                        image.drawRectangle(new cv.Rect(xMin, yMin, width, height,), color);
                        image.putText(`${confidence * 100}%`, new cv.Point2(xMin, yMin -7),
                            cv.FONT_HERSHEY_COMPLEX, 1, color, 1)
                    }
                )
            )
            return image;
        }
    ).then(
        (image) => cv.imwrite(options.output, image)
    );


    return 0;
}

main();
