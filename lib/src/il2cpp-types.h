// ReSharper disable CppClangTidyClangDiagnosticReservedIdentifier
// ReSharper disable CppClangTidyBugproneReservedIdentifier

#pragma once

namespace Genshin {

    struct ByteArray {
        void* klass;
        void* monitor;
        void* bounds;
        uint64_t max_length;
        uint8_t vector[32];
    };
}
