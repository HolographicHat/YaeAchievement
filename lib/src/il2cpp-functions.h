using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

DO_APP_FUNC(0x1929870, 0x1922c30, void, SetVersion, (void* obj, Il2CppString* value, void* method));

DO_APP_FUNC(0x6e7d020, 0x6e38a10, ByteArray*, RecordUserData, (int32_t nType));

DO_APP_FUNC(0x2b13f40, 0x2b09ce0, void, XorEncrypt, (ByteArray** data, int length, void* method));

DO_APP_FUNC(0x2d79560, 0x2d6e6f0, bool, KcpRecv, (void* client, ClientKcpEvent* evt, void* method));

DO_APP_FUNC(0x20004f0, 0x1ff8530, VOID, RequestLogin, (LPVOID obj, LPVOID token, UINT uid));

DO_APP_FUNC(0x130ead0, 0x1b90950, VOID, SetChecksum, (LPVOID obj, Il2CppString* value));

DO_APP_FUNC(0x2266e40, 0x225eb50, VOID, ForceQuit, (LPVOID obj));

DO_APP_FUNC(0x60af410, 0x60735f0, LPVOID, GetSingletonManager, ());

DO_APP_FUNC(0x60af140, 0x6073320, LPVOID, GetSingletonInstance, (LPVOID obj, Il2CppString* value));
