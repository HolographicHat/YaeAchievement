using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

DO_APP_FUNC(0x728c160, 0x71cc2e0, Il2CppString*, ToBase64String, (ByteArray* value, int offset, int length, void* method));

DO_APP_FUNC(0x2e2c930, 0x2dc4b90, void, SetVersion, (void* obj, Il2CppString* value, void* method));

DO_APP_FUNC(0x7c318d0, 0x7b69060, ByteArray*, RecordUserData, (int32_t nType));

DO_APP_FUNC(0x1ba7d30, 0x1b7b9f0, void, XorEncrypt, (ByteArray** data, int length, void* method));

DO_APP_FUNC(0xc3fe80, 0xc47280, int, KcpSend, (void* client, KcpPacket* pkt, void* method));

DO_APP_FUNC(0xf1ec70, 0xf1bca0, bool, KcpRecv, (void* client, ClientKcpEvent* evt, void* method));

DO_APP_FUNC(0x75a6880, 0x74e4b80, LPVOID, GetDefaultEncoding, ());

DO_APP_FUNC(0x75a6130, 0x74e4420, Il2CppString*, GetString, (LPVOID encoding, LPVOID bytes));

DO_APP_FUNC(0x75a4fc0, 0x74e32b0, ByteArray*, GetBytes, (LPVOID encoding, LPVOID str));

DO_APP_FUNC(0x1bf31f0, 0x1bc5f60, VOID, RequestLogin, (LPVOID obj, LPVOID token, UINT uid));

DO_APP_FUNC(0x4922d40, 0x4879590, VOID, SetChecksum, (LPVOID obj, Il2CppString* value));

DO_APP_FUNC(0x34780d0, 0x3401460, VOID, ForceQuit, (LPVOID obj));

DO_APP_FUNC(0x57df820, 0x5727410, LPVOID, GetSingletonManager, ());

DO_APP_FUNC(0x57df550, 0x5727140, LPVOID, GetSingletonInstance, (LPVOID obj, Il2CppString* value));
