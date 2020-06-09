const getUrlParam =
    (key) => {
      let searchParams = new URLSearchParams(location.search);
      return searchParams.get(key);
    }

const hasUrlParam =
    (key) => {
      let searchParams = new URLSearchParams(location.search);
      return searchParams.has(key);
    }

let ub = getUrlParam('b');
let us = getUrlParam('s');
let um = getUrlParam('m');

let currentBackend = getSearchParamsBackend();
let currentModel = getSearchParamsModel();
let currentDevice = getSearchParamsPlugin();

const checkedModelStyle =
    () => {
      if (um) {
        if (um.includes(' ')) {
          let umArray = um.split(' ');
          for (let modelName of umArray) {
            let modelClass = $('#' + modelName).parent().parent().attr('id');
            $('.model[id=' + modelClass + '] input').removeAttr('checked');
            $('.model[id=' + modelClass + '] label').removeClass('checked');
            $('#' + modelName).attr('checked', 'checked');
            $('#l-' + modelName).addClass('checked');
          }
        } else {
          $('.model input').removeAttr('checked');
          $('.model label').removeClass('checked');
          $('#' + um).attr('checked', 'checked');
          $('#l-' + um).addClass('checked');
        }
      }
    }

const disableModel =
    () => {
      if (um) {
        if (um.includes(' ')) {
          let umArray = um.split(' ');
          for (let modelName of umArray) {
            let modelClass = $('#' + modelName).parent().parent().attr('id');
            $('.model[id=' + modelClass + '] input').attr('disabled', false);
            $('.model[id=' + modelClass + '] label')
                .removeClass('cursordefault');
            $('#' + modelName).attr('disabled', true);
            $('#l-' + modelName).addClass('cursordefault');
          }
        } else {
          $('.model input').attr('disabled', false);
          $('.model label').removeClass('cursordefault');
          $('#' + um).attr('disabled', true);
          $('#l-' + um).addClass('cursordefault');
        }
      }
    }

let singleModelTable =
    (modelList, modelClass = 'model', showName = 'Model') => {
      const allFormats = new Set(modelList.map((m) => m.format));
      const format = [...allFormats][0];
      const tbody = $('#query tbody');
      const trows = [];

      const trow = $(`<tr class='model' id='${modelClass}'>`);
      const tdata = $('<td>');
      trow.append(tdata);

      const models =
          modelList.filter((m) => format === m.format && !m.disabled);
      for (const model of models) {
        const modelName = model.modelName.replace(/ \(.*\)$/, '');
        const modelId = model.modelId;
        tdata.append($(`<input type='radio' class='d-none' name='m' id='${
            modelId}' value='${modelId}'>`));
        tdata.append($(`<label id='l-${modelId}' class='themodel' for='${
            modelId}' title='${modelName}'>${modelName}</label>`));
      }
      trows.push(trow);

      trows[0].prepend($(`<th class='text-center' rowspan='${allFormats.size}'>
                      <a href='../model.html' title='View model details'>${
          showName}</a>
                    </th>`));
      tbody.prepend(trows);
      return allFormats;
    }

// construct the device buttons in html. 
// The first device in the deviceArray will be used as default
let configureDevice =
    (deviceArray) => {
      const brows = $('.device');

      for (const device of deviceArray) {
        const deviceName = device.replace(/ \(.*\)$/, '');
        const deviceLow = deviceName.toLowerCase();

        brows.append($(`<input type='radio' name='bw' class='d-none' id='${
            deviceLow}' value='${deviceName}'>`));
        brows.append($(`<label id='l-${deviceLow}' for='${
            deviceLow}' class='lml'>${deviceName}</label>`));
      }

      currentDevice = deviceArray[0].toLowerCase();
      up = currentDevice

      return
    }

let getModelClasss =
    () => {
      let ids = [];
      for (let model of $('#query tbody .model')) {
        ids.push(model.id);
      }
      return [...new Set(ids)];
    }

const changeModel = () => {
  $('.alert').hide();
  um = $('input:radio[name="m"]:checked').attr('id');
  if (currentModel === um) {
    return;
  }
  currentModel = um;

  let modelClasss = getModelClasss();
  let seatModelClass = $('#' + um).parent().parent().attr('id');
  if (modelClasss.length > 1) {
    for (let modelClass of modelClasss) {
      if (seatModelClass !== modelClass) {
        let modelName =
            $('.model[id=' + modelClass + '] input:radio[checked="checked"]')
                .attr('id');
        if (typeof modelName !== 'undefined') {
          um = um + '+' + modelName;
        }
      }
    }

    currentModel = um;
  }

  if (currentBackend && currentDevice) {
    strsearch = `?b=${currentBackend}&plugin=${currentDevice}&m=${um}&s=${us}`;
  } else {
    strsearch = `?b=${ub}&plugin=${up}&m=${um}&s=${us}`;
  }

  window.history.pushState(null, null, strsearch);

  checkedModelStyle();
  disableModel();
  updateTitle('Image Classification', currentDevice, currentModel);
  $('.offload').hide();

  main(us === 'camera');
};

const changePlugin =
    () => {
      $('.alert').hide();
      up = $('input:radio[name="bw"]:checked').attr('id');

      if (currentDevice === up || progress) {
        return;
      }

      $('#' + currentDevice).removeAttr('checked', 'checked');
      $('#l-' + currentDevice).removeClass('checked');
      $('#' + up).attr('checked', 'checked');
      $('#l-' + up).addClass('checked');

      currentDevice = up;
      if (currentBackend && currentDevice) {
        strsearch =
            `?b=${currentBackend}&plugin=${currentDevice}&m=${um}&s=${us}`;
      } else {
        strsearch = `?b=${ub}&plugin=${up}&m=${um}&s=${us}`;
      }

      window.history.pushState(null, null, strsearch);

      updateTitle('Image Classification', up, currentModel);
      main(us === 'camera');
    }

const showAlert =
    (error) => {
      console.error(error);
      let div = document.createElement('div');
      div.setAttribute(
          'class',
          'backendAlert alert alert-warning alert-dismissible fade show');
      div.setAttribute('role', 'alert');
      div.innerHTML = `<strong>${error}</strong>`;
      div.innerHTML +=
          `<button type='button' class='close' data-dismiss='alert' aria-label='Close'><span aria-hidden='true'>&times;</span></button>`;
      let container = document.getElementById('container');
      container.insertBefore(div, container.firstElementChild);
    }

const updateTitle =
    (name, device, modelId) => {
      let nameText = name;
      let deviceText = device.toUpperCase();
      let modelText = getModel(modelId).modelName;

      $('#ictitle')
          .html(`${nameText} / ${deviceText} / ${modelText || 'None'}`);
    }

const updateProgress =
    (ev) => {
      if (ev.lengthComputable) {
        let totalSize = ev.total / (1000 * 1000);
        let loadedSize = ev.loaded / (1000 * 1000);
        let percentComplete = ev.loaded / ev.total * 100;
        percentComplete = percentComplete.toFixed(0);
        progressBar.style = `width: ${percentComplete}%`;
        $('.loading-page .counter h1')
            .html(`${loadedSize.toFixed(1)}/${totalSize.toFixed(1)}MB ${
                percentComplete}%`);
      }
    }

async function loadUrl(url, binary) {
  return new Promise((resolve, reject) => {
    if (this.outstandingRequest) {
      this.outstandingRequest.abort();
    }
    let request = new XMLHttpRequest();
    this.outstandingRequest = request;
    request.open('GET', url, true);
    if (binary) {
      request.responseType = 'arraybuffer';
    }

    request.onload = function(ev) {
      if (request.readyState === 4) {
        if (request.status === 200) {
          resolve(request.response);
        } else {
          reject(new Error(
              'Failed to load ' + url + ' status: ' + request.status));
        }
      }
    };

    if (progress) {
      request.onprogress = updateProgress;
    }

    request.send();
  });
}

const showProgress =
    async (pm, pb, pi) => {
  let p = '';
  let modelicon = ``;
  if (pm === 'done') {
    modelicon = `<svg class='prog_list_icon' viewbox='0 0 24 24'>
                  <path class='st0' d='M12 20c4.4 0 8-3.6 8-8s-3.6-8-8-8-8 3.6-8 8 3.6 8 8 8zm0 1.5c-5.2 0-9.5-4.3-9.5-9.5S6.8 2.5 12 2.5s9.5 4.3 9.5 9.5-4.3 9.5-9.5 9.5z'></path>
                  <path class='st0' d='M11.1 12.9l-1.2-1.1c-.4-.3-.9-.3-1.3 0-.3.3-.4.8-.1 1.1l.1.1 1.8 1.6c.1.1.4.3.7.3.2 0 .5-.1.7-.3l3.6-4.1c.3-.3.4-.8.1-1.1l-.1-.1c-.4-.3-1-.3-1.3 0l-3 3.6z'></path>
                </svg>`;
  } else if (pm === 'current') {
    modelicon = `<svg class='prog_list_icon prog_list_icon-${
        pb}' width='24' height='24' viewbox='0 0 24 24'>
                <path d='M12.2 20a8 8 0 1 0 0-16 8 8 0 0 0 0 16zm0 1.377a9.377 9.377 0 1 1 0-18.754 9.377 9.377 0 0 1 0 18.754zm-4-8a1.377 1.377 0 1 1 0-2.754 1.377 1.377 0 0 1 0 2.754zm4 0a1.377 1.377 0 1 1 0-2.754 1.377 1.377 0 0 1 0 2.754zm4 0a1.377 1.377 0 1 1 0-2.754 1.377 1.377 0 0 1 0 2.754z' fill='#006DFF' fill-rule='evenodd'></path>
              </svg>`;
  } else {
    modelicon = `<svg class='prog_list_icon prog_list_icon-${
        pi}' width='24' height='24' viewbox='0 0 24 24'>
                <path d='M12 16.1c1.8 0 3.3-1.4 3.3-3.2 0-1.8-1.5-3.2-3.3-3.2s-3.3 1.4-3.3 3.2c0 1.7 1.5 3.2 3.3 3.2zm0 1.7c-2.8 0-5-2.2-5-4.9S9.2 8 12 8s5 2.2 5 4.9-2.2 4.9-5 4.9z'></path>
              </svg>`;
  }

  let updateicon = ``;
  if (pb === 'done') {
    updateicon = `<svg class='prog_list_icon' viewbox='0 0 24 24'>
                  <path class='st0' d='M12 20c4.4 0 8-3.6 8-8s-3.6-8-8-8-8 3.6-8 8 3.6 8 8 8zm0 1.5c-5.2 0-9.5-4.3-9.5-9.5S6.8 2.5 12 2.5s9.5 4.3 9.5 9.5-4.3 9.5-9.5 9.5z'></path>
                  <path class='st0' d='M11.1 12.9l-1.2-1.1c-.4-.3-.9-.3-1.3 0-.3.3-.4.8-.1 1.1l.1.1 1.8 1.6c.1.1.4.3.7.3.2 0 .5-.1.7-.3l3.6-4.1c.3-.3.4-.8.1-1.1l-.1-.1c-.4-.3-1-.3-1.3 0l-3 3.6z'></path>
                </svg>`;
  } else if (pb === 'current') {
    updateicon = `<svg class='prog_list_icon prog_list_icon-${
        pb}' width='24' height='24' viewbox='0 0 24 24'>
                <path d='M12.2 20a8 8 0 1 0 0-16 8 8 0 0 0 0 16zm0 1.377a9.377 9.377 0 1 1 0-18.754 9.377 9.377 0 0 1 0 18.754zm-4-8a1.377 1.377 0 1 1 0-2.754 1.377 1.377 0 0 1 0 2.754zm4 0a1.377 1.377 0 1 1 0-2.754 1.377 1.377 0 0 1 0 2.754zm4 0a1.377 1.377 0 1 1 0-2.754 1.377 1.377 0 0 1 0 2.754z' fill='#006DFF' fill-rule='evenodd'></path>
              </svg>`;
  } else {
    updateicon = `<svg class='prog_list_icon prog_list_icon-${
        pi}' width='24' height='24' viewbox='0 0 24 24'>
                <path d='M12 16.1c1.8 0 3.3-1.4 3.3-3.2 0-1.8-1.5-3.2-3.3-3.2s-3.3 1.4-3.3 3.2c0 1.7 1.5 3.2 3.3 3.2zm0 1.7c-2.8 0-5-2.2-5-4.9S9.2 8 12 8s5 2.2 5 4.9-2.2 4.9-5 4.9z'></path>
              </svg>`;
  }

  let inferenceicon = ``;
  if (pi === 'done') {
    inferenceicon = `<svg class='prog_list_icon' viewbox='0 0 24 24'>
                  <path class='st0' d='M12 20c4.4 0 8-3.6 8-8s-3.6-8-8-8-8 3.6-8 8 3.6 8 8 8zm0 1.5c-5.2 0-9.5-4.3-9.5-9.5S6.8 2.5 12 2.5s9.5 4.3 9.5 9.5-4.3 9.5-9.5 9.5z'></path>
                  <path class='st0' d='M11.1 12.9l-1.2-1.1c-.4-.3-.9-.3-1.3 0-.3.3-.4.8-.1 1.1l.1.1 1.8 1.6c.1.1.4.3.7.3.2 0 .5-.1.7-.3l3.6-4.1c.3-.3.4-.8.1-1.1l-.1-.1c-.4-.3-1-.3-1.3 0l-3 3.6z'></path>
                </svg>`;
  } else if (pi === 'current') {
    inferenceicon = `<svg class='prog_list_icon prog_list_icon-${
        pb}' width='24' height='24' viewbox='0 0 24 24'>
                <path d='M12.2 20a8 8 0 1 0 0-16 8 8 0 0 0 0 16zm0 1.377a9.377 9.377 0 1 1 0-18.754 9.377 9.377 0 0 1 0 18.754zm-4-8a1.377 1.377 0 1 1 0-2.754 1.377 1.377 0 0 1 0 2.754zm4 0a1.377 1.377 0 1 1 0-2.754 1.377 1.377 0 0 1 0 2.754zm4 0a1.377 1.377 0 1 1 0-2.754 1.377 1.377 0 0 1 0 2.754z' fill='#006DFF' fill-rule='evenodd'></path>
              </svg>`;
  } else {
    inferenceicon = `<svg class='prog_list_icon prog_list_icon-${
        pi}' width='24' height='24' viewbox='0 0 24 24'>
                <path d='M12 16.1c1.8 0 3.3-1.4 3.3-3.2 0-1.8-1.5-3.2-3.3-3.2s-3.3 1.4-3.3 3.2c0 1.7 1.5 3.2 3.3 3.2zm0 1.7c-2.8 0-5-2.2-5-4.9S9.2 8 12 8s5 2.2 5 4.9-2.2 4.9-5 4.9z'></path>
              </svg>`;
  }

  p = `
    <nav class='prog'>
      <ul class='prog_list'>
        <li class='prog prog-${pm}'>
          ${modelicon}<span class='prog_list_title'>Model loading</span>
        </li>
        <li class='prog prog-${pb}'>
          ${updateicon}<span class='prog_list_title'>Model compilation</span>
        </li>
        <li class='prog prog-${pi}'>
          ${
      inferenceicon}<span class='prog_list_title'>Model inferencing</span>
        </li>
      </ul>
    </nav>
  `;

  $('#progressmodel').show();
  $('#progressstep').html(p);
  $('.shoulddisplay').hide();
  $('.icdisplay').hide();
  $('#resulterror').hide();
  await new Promise(res => setTimeout(res, 100));
}

const showResultUI = () => {
  $('#progressmodel').hide();
  $('.icdisplay').fadeIn();
  $('.shoulddisplay').fadeIn();
  $('#resulterror').hide();
}