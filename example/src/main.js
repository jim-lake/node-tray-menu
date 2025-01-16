const { app, BrowserWindow } = require('electron');
const path = require('node:path');
const { MenuItem, Menu, Tray } = require('../../dist/index');

const RESOURCE_PATH =
  process.env.NODE_ENV === 'development'
    ? path.join(_getDebugRoot(), 'resources')
    : path.join(process.resourcesPath, 'resources');

const createWindow = () => {
  const mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      preload: MAIN_WINDOW_PRELOAD_WEBPACK_ENTRY,
    },
  });
  mainWindow.loadURL(MAIN_WINDOW_WEBPACK_ENTRY);
};

app.whenReady().then(() => {
  createWindow();

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow();
    }
  });

  _makeTray();
});

let g_items;
let g_menu;
let g_tray;

function _makeTray() {
  console.log('simple example start');

  console.log('making menu items');
  g_items = [
    new MenuItem({ type: 'normal', label: 'Item 1', click: _click }),
    new MenuItem({ type: 'normal', label: 'Item 2', click: _click2 }),
    new MenuItem({ type: 'normal', label: 'Disabled' }),
    new MenuItem({ type: 'separator' }),
    new MenuItem({ type: 'normal', label: 'Item 3', click: _click3 }),
  ];

  console.log('making menu');
  g_menu = new Menu();
  console.log('menu:', g_menu);

  console.log('adding menu items to menu');
  g_items.forEach((mi) => g_menu.append(mi));

  const file = getResourcePath('icon.png');
  console.log('making tray with file:', file, 'and menu:', g_menu);
  g_tray = new Tray(file, g_menu);

  console.log('created:', g_tray);
  setInterval(_tick, 100);
  setInterval(_modify, 2000);
}
function _tick() {
  const item0 = g_menu.items[0];
  if (item0) {
    item0.label = 'Item 1: ' + Date.now();
  }
}
function _modify() {
  if (Math.random() > 0.51) {
    const pos = Math.floor(Math.random() * g_menu.items.length);
    console.log('_modify: remove:', pos);
    g_menu.remove(pos);
  } else {
    const now = Date.now();
    const mi = new MenuItem({
      type: 'normal',
      label: 'Add ' + now,
      click: _customClick.bind(null, now),
    });
    const pos = Math.floor(Math.random() * g_menu.items.length);
    g_menu.insert(pos, mi);
  }
}
function _click() {
  console.log('_click');
}
function _click2() {
  console.log('_click2');
}
function _click3() {
  console.log('_click3');
}
function _customClick(time) {
  console.log('_customClick:', time);
}

function getResourcePath(file) {
  return path.join(RESOURCE_PATH, file);
}
function _getDebugRoot() {
  const parts = __dirname.split('/');
  while (parts.length > 0) {
    const last = parts.pop();
    if (!last || last === 'node_modules' || last === '.webpack') {
      break;
    }
  }
  return parts.join('/');
}
