const {postProcessing} = require('../../lib/inference-engine-node');

function showResults(results, labels) {
  const class_header = labels ? 'class' : 'classId'
  console.log(`${class_header}`.padEnd(10) + 'probability'.padEnd(15) + 'box');
  const header = '-------';
  console.log(header.padEnd(10) + header.padEnd(15) + header);
  results.forEach(result => {
    const classId = result.classId.toString();
    let label = labels ? labels[classId - 1] : classId;
    const probability = result.probability.toFixed(5).toString();
    const x1 = result.minX.toFixed(2).toString();
    const y1 = result.minY.toFixed(2).toString();
    const x2 = result.maxX.toFixed(2).toString();
    const y2 = result.maxY.toFixed(2).toString();
    console.log(
        label.padEnd(10) + probability.toString().padEnd(15) +
        `[${x1},${y1},${x2},${y2}]`);
  })
}

function topSSDResults(tensor, threshold = 0.5, dims) {
  const result = [];
  const classIdIndex = 1;
  const predictionIndex = 2;
  const x1Index = 3;
  const y1Index = 4;
  const x2Index = 5;
  const y2Index = 6;
  const dataBoxLength = dims[dims.length - 1];

  for (let b = 0; b < dims[0]; b++) {
    const batchOffset = b * dims[2];
    for (let i = 0; i < dims[2]; i++) {
      const curProposal = batchOffset + dataBoxLength * i;

      const prediction = tensor[curProposal + predictionIndex];

      if (prediction && prediction >= threshold) {
        const classId = tensor[curProposal + classIdIndex];
        const x1 = tensor[curProposal + x1Index];
        const y1 = tensor[curProposal + y1Index];
        const x2 = tensor[curProposal + x2Index];
        const y2 = tensor[curProposal + y2Index];
        result.push(new postProcessing.ObjectDetectionPrediction(
            x1, y1, x2, y2, prediction, classId));
      }
    }
  }
  return result;
}


module.exports = {
  showResults,
  topSSDResults
};
