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