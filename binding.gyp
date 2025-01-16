{
  "targets": [
    {
      "target_name": "node_tray_menu",
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "sources": ["src/mac/main.cc", "src/mac/TrayMenu.mm"],
      "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
      "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
      "link_settings": {
        "libraries": ["-framework Foundation", "-framework AppKit"]
      }
    }
  ]
}
