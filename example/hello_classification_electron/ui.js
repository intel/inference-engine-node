const ie = require('inference-engine-node');

let strsearch;
let skeletonDetectionPath =
    location.pathname.toLocaleLowerCase().indexOf('skeleton_detection');

const InputElement = document.getElementById('input');
const ImageElement = document.getElementById('image');

if (!location.search) {
  strsearch = `?b=openvinojs&plugin=none&m=none&s=image`;
  let path = location.href;
  location.href = path + strsearch;
}

/**
 * UI for switch Image and Live Camera
 */

$(document).ready(() => {
  $('#img').click(() => {
    us = 'image';
    if (currentBackend && currentDevice && currentModel) {
      strsearch = `?b=${currentBackend}&plugin=${currentDevice}&m=${
          currentModel}&s=${us}`;
    } else {
      strsearch = `?b=${ub}&plugin=${up}&m=${um}&s=${us}`;
    }

    window.history.pushState(null, null, strsearch);
    updateScenario();
  });

  $('#cam').click(() => {
    us = 'camera';
    strsearch = `?s=${us}`;
    window.history.pushState(null, null, strsearch);
    updateScenario(true);
  });
});

$(document).ready(() => {
  if (us == 'camera') {
    $('.nav-pills li').removeClass('active');
    $('.nav-pills #cam').addClass('active');
    $('#imagetab').removeClass('active');
    $('#cameratab').addClass('active');
  } else {
    $('.nav-pills li').removeClass('active');
    $('.nav-pills #img').addClass('active');
    $('#cameratab').removeClass('active');
    $('#imagetab').addClass('active');
    $('#fps').html('');
  }
});

$(document).ready(() => {
  $('#img').click(() => {
    $('.alert').hide();
    $('#fps').html('');
    $('ul.nav-pills li').removeClass('active');
    $('ul.nav-pills #img').addClass('active');
    $('#imagetab').addClass('active');
    $('#cameratab').removeClass('active');
  });

  $('#cam').click(() => {
    $('.alert').hide();
    $('ul.nav-pills li').removeClass('active');
    $('ul.nav-pills #cam').addClass('active');
    $('#cameratab').addClass('active');
    $('#imagetab').removeClass('active');
  });
});

$(document).ready(() => {
  availableDevice = ie.createCore().getAvailableDevices()
  console.log('Get availabelDevice')
  console.log(availableDevice)
  // The defalut Device is the first in the availableDevice
  constructDevice(availableDevice)
  $('#' + up).attr('checked', 'checked');
  $('#l-' + up).addClass('checked');
});

const showError =
    (title, description) => {
      $('#progressmodel').hide();
      progress = false;
      $('.icdisplay').hide();
      $('.shoulddisplay').hide();
      $('#resulterror').fadeIn();
      if (title && description) {
        $('.errortitle').html(title);
        $('.errordescription').html(description);
      } else {
        $('.errortitle').html('Prediction Failed');
        $('.errordescription').html('Please check error log for more details');
      }
    }

/**
 * UI for model table at the head of HTML
 */
const constructModelTable = (modelLists, multiple = false) => {
  let formatTypes = [];
  let formats = singleModelTable(modelLists);
  formatTypes.push(...formats);

  $('input:radio[name=m]').click(changeModel);
  checkedModelStyle();
  disableModel();
};

$(document).ready(() => {
  constructModelTable(imageClassificationModels);

  // UI for switching plugin
  $('input:radio[name=bw]').click(changePlugin);

  InputElement.addEventListener('change', (e) => {
    let files = e.target.files;
    if (files.length > 0 && us === 'image') {
      imageElement.src = URL.createObjectURL(files[0]);
    }
  }, false);

  ImageElement.addEventListener('load', () => {
    if (us === 'image') {
      predictImage(imageElement);
    }
  });
});

$(window).ready(() => {
  // currentDevice = up;
  currentBackend = ub;
  currentModel = um;

  if (um === 'none') {
    showError(
        'No model selected', 'Please select a model to start prediction.');
    return;
  }

  updateTitle('Image Classification', currentDevice, currentModel);

  main(us === 'camera');
});
