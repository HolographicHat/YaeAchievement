{
  "targets": [
    {
      "target_name": "native",
      "sources": [
        "src/homu.h",
        "src/main.cc",
        "src/utils.h",
        "src/utils.cc",
        "src/define.h",
        "src/homu.cpp",
        "src/wmi/wmi.cpp",
        "src/wmi/wmi.hpp",
        "src/wmi/unistd.h",
        "src/VMProtectSDK.h",
        "src/wmi/wmiresult.cpp",
        "src/wmi/wmiresult.hpp",
        "src/wmi/wmiclasses.hpp",
        "src/wmi/wmiexception.hpp",
        "src/registry/registry.hpp"
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
            "-sdl",
            "-O2",
            "-Ot",
            "-Oi",
            "-GL",
            "-Gw"
          ]
        }
      }
    }
  ]
}