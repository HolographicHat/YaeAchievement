#pragma once

#pragma region IL2CPPInternalTypes

typedef uint16_t Il2CppChar;
typedef uintptr_t il2cpp_array_size_t;
typedef int32_t il2cpp_array_lower_bound_t;

typedef struct Il2CppObject {
    union {
        void* klass;
        void* vtable;
    } Il2CppClass;
    void* monitor;
} Il2CppObject;

typedef struct Il2CppString {
    Il2CppObject object;
    int32_t length;
    Il2CppChar chars[32];
} Il2CppString;

typedef struct Il2CppArrayBounds {
    il2cpp_array_size_t length;
    il2cpp_array_lower_bound_t lower_bound;
} Il2CppArrayBounds;

#pragma endregion

namespace Genshin {

    struct ByteArray {
        void* klass;
        void* monitor;
        Il2CppArrayBounds* bounds;
        il2cpp_array_size_t max_length;
        uint8_t vector[32];
    };

    struct KcpPacket {
        BYTE* data;
        UINT32 length;
    };

    enum class KcpEventType : int {
        EventNotSet = -1,
        EventConnect = 0,
        EventConnectFailed = 1,
        EventDisconnect = 2,
        EventRecvMsg = 3,
        EventCount = 4,
    };

    struct KcpEvent_Fields {
        KcpEventType type;
        UINT32 token;
        UINT32 data;
        KcpPacket* packet;
    };

    struct ClientKcpEvent {
        KcpEvent_Fields fields;
    };

}
