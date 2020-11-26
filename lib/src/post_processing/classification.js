function topClassificationResults(tensor, labels, k) {
    let probabilities = Array.from(tensor);
    let indexes = probabilities.map((prob, index) => [prob, index]);
    let sorted = indexes.sort((a, b) => {
        if (a[0] === b[0]) {
            return 0;
        }
        return a[0] < b[0] ? -1 : 1;
    });
    sorted.reverse();
    let classes = [];
    for (let i = 0; i < k; ++i) {
        const prob = sorted[i][0];
        const index = sorted[i][1];
        const c = {
            id: index.toString(),
            label: labels ? labels[index] : '',
            prob: prob.toFixed(6)
        };
        classes.push(c);
    }
    return classes;
}

module.exports = {
    topClassificationResults
}
