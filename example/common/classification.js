function showResults(results) {
  console.log('id of class'.padEnd(10) + 'probability'.padEnd(15) + 'label');
  const header = '-------';
  console.log(header.padEnd(10) + header.padEnd(15) + header);
  results.forEach(result => {
    console.log(result.id.padEnd(10) + result.prob.padEnd(15) + result.label);
  })
}


module.exports = {showResults};
