using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

DO_APP_FUNC(0x258fd40, 0x2548e50, void, SetVersion, (void* obj, Il2CppString* value, void* method));

DO_APP_FUNC(0x7c6d0d0, 0x7bde850, ByteArray*, RecordUserData, (int32_t nType));

DO_APP_FUNC(0x3220f00, 0x31c1650, void, XorEncrypt, (ByteArray** data, int length, void* method));

DO_APP_FUNC(0x12f5df0, 0x12ddd80, bool, KcpRecv, (void* client, ClientKcpEvent* evt, void* method));

DO_APP_FUNC(0x19b6f30, 0x198d6c0, VOID, RequestLogin, (LPVOID obj, LPVOID token, UINT uid));

DO_APP_FUNC(0x29b18f0, 0x2960ec0, VOID, SetChecksum, (LPVOID obj, Il2CppString* value));

DO_APP_FUNC(0x264ee90, 0x2606720, VOID, ForceQuit, (LPVOID obj));

DO_APP_FUNC(0x624d630, 0x61bd630, LPVOID, GetSingletonManager, ());

DO_APP_FUNC(0x624d360, 0x61bd360, LPVOID, GetSingletonInstance, (LPVOID obj, Il2CppString* value));
