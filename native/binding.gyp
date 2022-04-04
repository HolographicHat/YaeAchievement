{
  "targets": [
    {
      "target_name": "native",
      "sources": [
        "main.cc",
        "utils.cc",
        "utils.h",
        "define.h"
      ],
      "cflags!": [
        "-fno-exceptions"
      ],
      "cflags_cc!": [
        "-fno-exceptions"
      ],
      "defines": [
        "NAPI_DISABLE_CPP_EXCEPTIONS"
      ],
      "include_dirs": [
        "<!(node -p \"require('node-addon-api').include_dir\")"
      ],
      "msvs_settings": {
        "VCCLCompilerTool": {
          "AdditionalOptions": [
            "-std:c++latest",
            "-DUNICODE",
            "-sdl"
          ]
        }
      }
    }
  ]
}