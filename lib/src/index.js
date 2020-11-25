const postProcessing = require('./post_processing')

module.exports = function(inferenceEngine) {
    inferenceEngine.postProcessing = postProcessing
    return inferenceEngine
}
