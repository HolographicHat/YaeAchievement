using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

DO_APP_FUNC(0x68df290, 0x6822570, Il2CppString*, ToBase64String, (ByteArray* value, int offset, int length, void* method));

DO_APP_FUNC(0x272fe00, 0x26df410, void, SetVersion, (void* obj, Il2CppString* value, void* method));

DO_APP_FUNC(0x710a730, 0x70401b0, ByteArray*, RecordUserData, (int32_t nType));

DO_APP_FUNC(0x26a7940, 0x2658ae0, void, XorEncrypt, (ByteArray** data, int length, void* method));

DO_APP_FUNC(0x19d99a0, 0x19b12f0, int, KcpSend, (void* client, KcpPacket* pkt, void* method));

DO_APP_FUNC(0x306e0e0, 0x3001bf0, bool, KcpRecv, (void* client, ClientKcpEvent* evt, void* method));

DO_APP_FUNC(0x6b832c0, 0x6ac2040, LPVOID, GetDefaultEncoding, ());

DO_APP_FUNC(0x6b82b50, 0x6ac18d0, Il2CppString*, GetString, (LPVOID encoding, LPVOID bytes));

DO_APP_FUNC(0x6b81980, 0x6ac0760, ByteArray*, GetBytes, (LPVOID encoding, LPVOID str));

DO_APP_FUNC(0x18a0490, 0x187a070, VOID, RequestLogin, (LPVOID obj, LPVOID token, UINT uid));

DO_APP_FUNC(0x3669cd0, 0x1b323c0, VOID, SetChecksum, (LPVOID obj, Il2CppString* value));

DO_APP_FUNC(0xed5d70, 0xecc910, VOID, ForceQuit, (LPVOID obj));

DO_APP_FUNC(0x57fb490, 0x574b7f0, LPVOID, GetSingletonManager, ());

DO_APP_FUNC(0x57fb1c0, 0x574b520, LPVOID, GetSingletonInstance, (LPVOID obj, Il2CppString* value));
