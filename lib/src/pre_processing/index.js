
function RGBAtoBGR(image, x, y, idx) {
    let i = Math.floor(idx / 4) * 3;
    input_data[i + 2] = image.bitmap.data[idx + 0];  // R
    input_data[i + 1] = image.bitmap.data[idx + 1];  // G
    input_data[i + 0] = image.bitmap.data[idx + 2];  // B
};