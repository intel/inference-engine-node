const {FaceDetectionRetailEngine} = require("./engines/faceDetectionRetailEngine");

const {Core} = require('../../lib/inference-engine-node');
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
        description: 'Required. Path to an .xml file with a trained model.'
    },
    {
        name: 'face_detection_device',
        type: String,
        defaultValue: 'CPU',
        description: 'Optional. Specify the target device to infer on ' +
            '(the list of available devices is shown below). ' +
            'Default value is CPU.'
    }
];

const commandLineArgs = require('command-line-args');
const commandLineUsage = require('command-line-usage');


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

    const core = new Core();

    const faceEngine = new FaceDetectionRetailEngine(core, options.face_detection_model)
    await faceEngine.initialize(options.face_detection_device)
    const image = cv.imread(options.image);
    const detectedFace = await faceEngine.process(image)

    detectedFace.forEach((face) => {
            const xmin = face.position[0]
            const ymin = face.position[1]
            const xmax = face.size[0]
            const ymax = face.size[1]
            image.drawRectangle(new cv.Rect(xmin, ymin, xmax, ymax,));
        }
    )
    cv.imwrite('out.jpg', image);

    return 0;
}

main();
