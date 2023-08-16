using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

DO_APP_FUNC(0x7974040, 0x792b560, Il2CppString*, ToBase64String, (ByteArray* value, int offset, int length, void* method));

DO_APP_FUNC(0x21e5c10, 0x21bf5c0, void, SetVersion, (void* obj, Il2CppString* value, void* method));

DO_APP_FUNC(0x81d1a00, 0x818d250, ByteArray*, RecordUserData, (int32_t nType));

DO_APP_FUNC(0x3174320, 0x3133880, void, XorEncrypt, (ByteArray** data, int length, void* method));

DO_APP_FUNC(0x2d49720, 0x2d103a0, int, KcpSend, (void* client, KcpPacket* pkt, void* method));

DO_APP_FUNC(0x3d6a920, 0x3d11e00, bool, KcpRecv, (void* client, ClientKcpEvent* evt, void* method));

DO_APP_FUNC(0x7c9c9c0, 0x7c56400, LPVOID, GetDefaultEncoding, ());

DO_APP_FUNC(0x7c9c220, 0x7c55c60, Il2CppString*, GetString, (LPVOID encoding, LPVOID bytes));

DO_APP_FUNC(0x7c9b030, 0x7c54a20, ByteArray*, GetBytes, (LPVOID encoding, LPVOID str));

DO_APP_FUNC(0x1d2e480, 0x1d0db20, VOID, RequestLogin, (LPVOID obj, LPVOID token, UINT uid));

DO_APP_FUNC(0x31285b0, 0x32ee9b0, VOID, SetChecksum, (LPVOID obj, Il2CppString* value));

DO_APP_FUNC(0xfa2eb0, 0xf99150, VOID, ForceQuit, (LPVOID obj));

DO_APP_FUNC(0x7257b20, 0x72079f0, LPVOID, GetSingletonManager, ());

DO_APP_FUNC(0x7257850, 0x7207720, LPVOID, GetSingletonInstance, (LPVOID obj, Il2CppString* value));
