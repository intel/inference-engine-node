// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// No Node.js APIs are available in this process because
// `nodeIntegration` is turned off. Use `preload.js` to
// selectively enable features needed in the rendering
// process.


const ie = require('../lib/ie');

ieVersion = ie.getVersion();
console.log(`Inference Engine version: `);
console.log(ieVersion);

const element = document.getElementById('ie-version');
if (element) {
  element.innerText = `${ieVersion.apiVersion.major}.${ieVersion.apiVersion.minor}`;
}
