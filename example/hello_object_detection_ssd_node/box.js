class ObjectDetectionPrediction {
  constructor(x1, y1, x2, y2, probability, classId) {
    this.x1 = x1;
    this.y1 = y1;
    this.x2 = x2;
    this.y2 = y2;
    this.probability = probability;
    this.classId = classId;
  }

  get minX() {
    return Math.min(this.x1, this.x2);
  }

  get minY() {
    return Math.min(this.y1, this.y2);
  }

  get maxX() {
    return Math.max(this.x1, this.x2);
  }

  get maxY() {
    return Math.max(this.y1, this.y2);
  }
}

module.exports.ObjectDetectionPrediction = ObjectDetectionPrediction;