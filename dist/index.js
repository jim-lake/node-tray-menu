const addon = require('../build/Release/node_tray_menu.node');
addon.setCallback(_callback);
const g_itemMap = new Map();
function _callback(index) {
  const mi = g_itemMap.get(index);
  mi?._click?.call?.(mi);
}
class MenuItem {
  constructor(params) {
    this._index = 0;
    this._label = '';
    this._label = params.label ?? '';
    this._click = params.click;
    const ret = addon.menuItemCreate(
      params.type,
      params.label,
      Boolean(params.click)
    );
    if (typeof ret === 'number') {
      this._index = ret;
      g_itemMap.set(this._index, this);
    } else {
      throw ret;
    }
  }
  get label() {
    return this._label;
  }
  set label(new_label) {
    addon.menuItemUpdate(this._index, new_label);
    this._label = new_label;
  }
  destroy() {
    addon.itemDestroy(this._index);
    g_itemMap.delete(this._index);
  }
}
class Menu {
  constructor() {
    this._index = 0;
    this.items = [];
    const ret = addon.menuCreate();
    if (typeof ret === 'number') {
      this._index = ret;
    } else {
      throw new Error(ret);
    }
  }
  append(menuItem) {
    this.insert(this.items.length, menuItem);
  }
  insert(pos, menuItem) {
    addon.menuInsertItem(this._index, menuItem._index, pos);
    this.items.splice(pos, 0, menuItem);
  }
  remove(pos) {
    addon.menuRemoveItem(this._index, pos);
    this.items.splice(pos, 1);
  }
  destroy() {
    addon.itemDestroy(this._index);
  }
}
class Tray {
  constructor(image, menu) {
    this._index = 0;
    const ret = addon.trayCreate(image, menu._index);
    if (typeof ret === 'number') {
      this._index = ret;
    } else {
      throw new Error(ret);
    }
  }
  destroy() {
    addon.trayDestroy(this._index);
  }
}
exports.MenuItem = MenuItem;
exports.Menu = Menu;
exports.Tray = Tray;
