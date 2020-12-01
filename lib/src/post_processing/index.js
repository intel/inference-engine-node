const classification = require('./classification');
const objectDetection = require('./object_detection');

module.exports = {
    ...classification,
    ...objectDetection
}
