// const ie = require('inference-engine-node');
const fs = require('fs').promises;

const canvasElement = document.getElementById('canvas');
const videoElement = document.getElementById('video');
const imageElement = document.getElementById('image');
const progressBar = document.getElementById('progressBar');
var canvasContext = canvasElement.getContext('2d');

var core;
var initialized = false;
var ie_net;
var exec_net;
var input_info;
var labels;
var output_info;
var progress = false;
var preOptions;
var postOptions;

let front = false;
let stats = new Stats();
let streaming = false;
let track;

function topResults(tensor, labels, k) {

  let temp = Array.from(tensor);
  let softmax = postOptions.softmax || false;
  let probs;

  if(softmax){
    let sum = temp.map(y => Math.exp(y)).reduce((a, b) => {return a+b})
    probs = temp.map((prob) => {return Math.exp(prob)/sum;});
  } else {
    probs = temp;
  }

  let indexes = probs.map((prob, index) => [prob, index]);
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

function showResults(results, time) {
  showResultUI();
  try {
    let inferenceTimeElement = document.getElementById('inferenceTime');
    inferenceTimeElement.innerHTML =
        `inference time: <span class='ir'>${time.toFixed(2)} ms</span>`;
  } catch (e) {
    console.log(e);
  }

  try {
    results.forEach((c, i) => {
      let labelElement = document.getElementById(`label${i}`);
      let probElement = document.getElementById(`prob${i}`);
      labelElement.innerHTML = `${c.label.split(',')[0]}`;
      probElement.innerHTML = `${(c.prob*100).toFixed(2)}%`;
    });
  } catch (e) {
    console.log(e);
  }

  return;
}

/**
 * Prepare Input Info before infer
 */
async function prepareInputInfo(image, input_data, width, height) {
  canvasContext.drawImage(
      image, 0, 0, canvasElement.width, canvasElement.height);
  if (canvasElement.width !== width || canvasElement.height !== height) {
    throw new Error(`canvas.width(${canvasElement.width}) is not ${
        width} or canvas.height(${canvasElement.height}) is not ${height}`);
  }

  let context = canvasElement.getContext('2d');
  let pixels = context.getImageData(0, 0, width, height).data;

  let channels = 3;
  const mean = preOptions.mean || [0, 0, 0, 0];
  const std = preOptions.std || [1, 1, 1, 1];
  const channelScheme = preOptions.channelScheme || 'RGB';
  const norm = preOptions.norm || false;
  const imageChannels = 4;

  if(norm) {
      pixels = new Float32Array(pixels).map(p => p / 255);
  }

  if (channelScheme === 'RGB') {
    for (let y = 0; y < height; ++y) {
      for (let x = 0; x < width; ++x) {
        for (let c = 0; c < channels; ++c) {
          let value = pixels[y*width*imageChannels + x*imageChannels + c];
          input_data[y*width*channels + x*channels + c] = (value - mean[c]) / std[c];
        }
      }
    }
  } else if (channelScheme === 'BGR') {
    for (let y = 0; y < height; ++y) {
      for (let x = 0; x < width; ++x) {
        for (let c = 0; c < channels; ++c) {
          let value = pixels[y*width*imageChannels + x*imageChannels + (channels-c-1)];
          input_data[y*width*channels + x*channels + c] = (value - mean[c]) / std[c];
        }
      }
    }
  }

  return;
}


/**
 * 
 * Common predict method for image mode and live camera
 * 
 * @param {imageElement} imageSource 
 */
async function predict(imageSource) {

  if (!initialized) {
    console.log('Network is not initialized.');
    return;
  }
  
  let infer_req = exec_net.createInferRequest();
  const input_blob = infer_req.getBlob(input_info.name());
  const input_data = new Float32Array(input_blob.wmap());
  
  prepareInputInfo(
      imageSource, input_data, input_info.getDims()[2],
      input_info.getDims()[3]);
  input_blob.unmap();

  let startTime = performance.now();
  await infer_req.startAsync();
  let inferenceTime = performance.now() - startTime;

  const output_blob = infer_req.getBlob(output_info.name());
  const output_data = new Float32Array(output_blob.rmap());

  const canvasResults = topResults(output_data, labels, 3);
 
  return {
    result: canvasResults,
    time: inferenceTime,
  };
}


/**
 *  Load model and UI control
 */
async function loadModel(model) {

  initialized = false;

  let model_path = model.modelFile;
  let lablePath = model.labelsFile;
  let model_inputSize = model.inputSize;
  canvasElement.height = model_inputSize[0];
  canvasElement.width = model_inputSize[1];
  canvasContext = canvasElement.getContext("2d");

  try {
    const data = await fs.readFile(lablePath, {encoding: 'utf-8'});
    labels = data.split('\n');
  } catch (error) {
    warning(error);
  }

  preOptions = model.preOptions || {};
  postOptions = model.postOptions || {};

  const re = /\.bin$/g
  const xml_path = model_path.replace(re, '.xml');

  $('#progressmodel').fadeIn();
  $('.icdisplay').hide();
  $('.shoulddisplay').hide();
  $('#resulterror').hide();
  progress = true;

  core = ie.createCore();
  try {
    $('.loading-page .counter h1').html(``);
    ie_net = await core.readNetwork(xml_path, model_path);

  } catch {
    console.log("Start to get files online");
    try {
      let onlineModelUrl = model.onlineFile;
      let onlineBinUrl = model.onlineBinFile;
      if(onlineModelUrl === '' || onlineBinFile === ''){
        $('#progressmodel').hide();
        $('#resulterror').fadeIn();
        progress = false;
          showAlert(
            new Error('Failed to load models. Please check local files or URLs'));
        return;
      }

      let modelTemp = await loadUrl(onlineModelUrl, false);
      let binTemp = await loadUrl(onlineBinUrl, true);
      let binBytes = new Uint8Array(binTemp);
      console.log("Get the model online");

      ie_net = await core.readNetworkFromData(modelTemp, binBytes.buffer);
      
    } catch (e) {
      $('#progressmodel').hide();
      $('#resulterror').fadeIn();
      progress = false;
      showAlert(
        new Error('Failed to load model. Please check the local xml files or your internets'));
      return;
    }
  }

  await showProgress('done', 'current', 'pending');
  
  const inputs_info = ie_net.getInputsInfo();
  const outputs_info = ie_net.getOutputsInfo();

  input_info = inputs_info[0];
  input_info.setLayout('nhwc');
  output_info = outputs_info[0];

  try{
    exec_net = await core.loadNetwork(ie_net, currentDevice.toUpperCase());
    initialized = true;
    $('#progressmodel').hide();
    $('.icdisplay').fadeIn();
    $('.shoulddisplay').fadeIn();
    progress = false;

    console.log('Model initialized');
  } catch (e) {
    $('#progressmodel').hide();
    $('#resulterror').fadeIn();
    progress = false;
    showAlert(
        new Error('Failed to load model on Device '+ currentDevice.toUpperCase() + '. Please check the device'));
    return;
  }
}

const startPredictCamera = async () => {
  if (streaming) {
    try {
      stats.begin();
      let value = await predict(videoElement);
      showResults(value.result, value.time);
      stats.end();
      setTimeout(startPredictCamera, 0);
    } catch (e) {
      errorHandler(e);
    }
  }
}

async function predictImage(ImageSource) {
  streaming = false;
  if (track) {
    track.stop();
  }

  if (!initialized) {
    console.log('Network is not initialized.');
    return;
  }

  await showProgress('done', 'done', 'current');
  try {
    let value = await predict(ImageSource);
    await showProgress('done', 'done', 'done');
    showResults(value.result, value.time);
  } catch (e) {
    showAlert(
        new Error('Inference Error. Please check your device or network'));
  }
}

async function predictCamera() {
  if (!initialized) {
    console.log('Network is not initialized.');
    return;
  }
  streaming = true;
  await showProgress('done', 'done', 'current');
  try {
    let stream = await navigator.mediaDevices.getUserMedia(
        {audio: false, video: {facingMode: (front ? 'user' : 'environment')}});
    video.srcObject = stream;
    track = stream.getTracks()[0];
    startPredictCamera();
    await showProgress('done', 'done', 'done');
  } catch (e) {
    console.log('Camera predict error');
    console.log(e);
  }
}

const predictPath =
    (camera) => {
      (!camera) ? predictImage(imageElement) : predictCamera();
    }

for (const modelListName in modelZoo) {
  window[modelListName] = modelZoo[modelListName];
}

const getModel = (id) => {
  for (const model of Object.values(modelZoo).flat()) {
    if (id === model.modelId) {
      return model;
    }
  }
  return {};
};

const updateScenario =
    async (camera = false) => {
  streaming = false;
  predictPath(camera);
}

const main = async (camera = false) => {
  streaming = false;
  await showProgress('current', 'pending', 'pending');
  try {
    let model = getModel(currentModel);
    await loadModel(model);
  } catch (e) {
    console.log(e);
  }
  predictPath(camera);
}
