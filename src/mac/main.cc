#include <napi.h>
#include <string>

#include "TrayMenu.h"

using namespace Napi;

static ThreadSafeFunction g_tsfn = NULL;

static auto napiCallback = [](Env env, Function cb, const void *index) {
  cb.Call({Number::New(env, (long)index)});
};
static void _callback(const long index) {
  if (g_tsfn != NULL) {
    g_tsfn.BlockingCall((void *)index, napiCallback);
  }
}
static Value _setCallback(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  Value ret = env.Undefined();
  if (info.Length() < 1) {
    ret = String::New(env, "Expected at least 1 arguments");
  } else if (!info[0].IsFunction()) {
    ret = String::New(env, "Expected function arg 0");
  } else {
    const auto cb = info[0].As<Function>();
    if (g_tsfn != NULL) {
      g_tsfn.Abort();
      g_tsfn.Release();
      g_tsfn = NULL;
    }
    g_tsfn = ThreadSafeFunction::New(env, cb, "TrayMenu Callback", 0, 1);
    setCallback(_callback);
  }
  return ret;
}
static Value _menuItemCreate(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  Value ret = env.Undefined();

  if (info.Length() < 1) {
    ret = String::New(env, "Expected at least 1 arguments");
  } else if (!info[0].IsString()) {
    ret = String::New(env, "Expected string arg 0");
  } else {
    const auto type = info[0].As<String>().Utf8Value();
    if (type == "separator") {
      ret = Number::New(env, MenuItemCreateSeparator());
    } else if (type == "normal") {
      if (info.Length() < 3) {
        ret = String::New(env, "Expected at least 3 arguments");
      } else if (!info[1].IsString()) {
        ret = String::New(env, "Expected string arg 1");
      } else if (!info[2].IsBoolean()) {
        ret = String::New(env, "Expected bool arg 2");
      } else {
        const auto title = info[1].As<String>().Utf8Value();
        const auto has_callback = info[2].As<Boolean>();
        ret = Number::New(env, MenuItemCreate(title, has_callback));
      }
    } else {
      ret = String::New(env, "Unsupported Menu Type");
    }
  }
  return ret;
}
static Value _menuItemUpdate(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  Value ret = env.Undefined();
  if (info.Length() < 2) {
    ret = String::New(env, "Expected at least 2 arguments");
  } else if (!info[0].IsNumber()) {
    ret = String::New(env, "Expected number arg 0");
  } else if (!info[1].IsString()) {
    ret = String::New(env, "Expected string arg 1");
  } else {
    const int index = info[0].As<Number>().Int32Value();
    const auto title = info[1].As<String>().Utf8Value();
    MenuItemUpdate(index, title);
  }
  return ret;
}
static Value _itemDestroy(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  Value ret = env.Undefined();
  if (info.Length() < 1) {
    ret = String::New(env, "Expected at least 1 argument");
  } else if (!info[0].IsNumber()) {
    ret = String::New(env, "Expected number arg 0");
  } else {
    const int index = info[0].As<Number>().Int32Value();
    ItemDestroy(index);
  }
  return ret;
}
static Value _menuCreate(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  const auto ret = MenuCreate();
  return Number::New(env, ret);
}
static Value _menuInsertItem(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  Value ret = env.Undefined();
  if (info.Length() < 3) {
    ret = String::New(env, "Expected at least 3 arguments");
  } else if (!info[0].IsNumber()) {
    ret = String::New(env, "Expected number arg 0");
  } else if (!info[1].IsNumber()) {
    ret = String::New(env, "Expected number arg 1");
  } else if (!info[2].IsNumber()) {
    ret = String::New(env, "Expected number arg 2");
  } else {
    const int menu_index = info[0].As<Number>().Int32Value();
    const int menu_item_index = info[1].As<Number>().Int32Value();
    const int index = info[2].As<Number>().Int32Value();
    MenuInsertItem(menu_index, menu_item_index, index);
  }
  return ret;
}
static Value _menuRemoveItem(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  Value ret = env.Undefined();
  if (info.Length() < 2) {
    ret = String::New(env, "Expected at least 3 arguments");
  } else if (!info[0].IsNumber()) {
    ret = String::New(env, "Expected number arg 0");
  } else if (!info[1].IsNumber()) {
    ret = String::New(env, "Expected number arg 1");
  } else {
    const int menu_index = info[0].As<Number>().Int32Value();
    const int index = info[1].As<Number>().Int32Value();
    MenuRemoveItem(menu_index, index);
  }
  return ret;
}
static Value _trayCreate(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  Value ret = env.Undefined();
  if (info.Length() < 2) {
    ret = String::New(env, "Expected at least 2 arguments");
  } else if (!info[0].IsString()) {
    ret = String::New(env, "Expected number arg 0");
  } else if (!info[1].IsNumber()) {
    ret = String::New(env, "Expected string arg 1");
  } else {
    const auto path = info[0].As<String>().Utf8Value();
    const int menu_index = info[1].As<Number>().Int32Value();
    ret = Number::New(env, TrayCreate(path, menu_index));
  }
  return ret;
}
static Value _trayDestroy(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  Value ret = env.Undefined();
  if (info.Length() < 1) {
    ret = String::New(env, "Expected at least 1 argument");
  } else if (!info[0].IsNumber()) {
    ret = String::New(env, "Expected number arg 0");
  } else {
    const int index = info[0].As<Number>().Int32Value();
    TrayDestroy(index);
  }
  return ret;
}

Object Init(Napi::Env env, Object exports) {
  exports.Set("setCallback", Function::New(env, _setCallback));

  exports.Set("itemDestroy", Function::New(env, _itemDestroy));

  exports.Set("menuItemCreate", Function::New(env, _menuItemCreate));
  exports.Set("menuItemUpdate", Function::New(env, _menuItemUpdate));

  exports.Set("menuCreate", Function::New(env, _menuCreate));
  exports.Set("menuInsertItem", Function::New(env, _menuInsertItem));
  exports.Set("menuRemoveItem", Function::New(env, _menuRemoveItem));

  exports.Set("trayCreate", Function::New(env, _trayCreate));
  exports.Set("trayDestroy", Function::New(env, _trayDestroy));

  return exports;
}
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
