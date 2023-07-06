using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

DO_APP_FUNC(0x728b850, 0x7247520, Il2CppString*, ToBase64String, (ByteArray* value, int offset, int length, void* method));

DO_APP_FUNC(0x215d110, 0x214d7a0, void, SetVersion, (void* obj, Il2CppString* value, void* method));

DO_APP_FUNC(0x7b4dc00, 0x7afe4e0, ByteArray*, RecordUserData, (int32_t nType));

DO_APP_FUNC(0x166e5d0, 0x1662f60, void, XorEncrypt, (ByteArray** data, int length, void* method));

DO_APP_FUNC(0xf71860, 0xf69870, int, KcpSend, (void* client, KcpPacket* pkt, void* method));

DO_APP_FUNC(0x433e670, 0x4315590, bool, KcpRecv, (void* client, ClientKcpEvent* evt, void* method));

DO_APP_FUNC(0x75627b0, 0x751a960, LPVOID, GetDefaultEncoding, ());

DO_APP_FUNC(0x7561ff0, 0x751a1c0, Il2CppString*, GetString, (LPVOID encoding, LPVOID bytes));

DO_APP_FUNC(0x7560d90, 0x7518f90, ByteArray*, GetBytes, (LPVOID encoding, LPVOID str));

DO_APP_FUNC(0x1349850, 0x13415f0, VOID, RequestLogin, (LPVOID obj, LPVOID token, UINT uid));

DO_APP_FUNC(0x11ab1d0, 0x3e9f710, VOID, SetChecksum, (LPVOID obj, Il2CppString* value));

DO_APP_FUNC(0xd4e730, 0xd49dd0, VOID, ForceQuit, (LPVOID obj));

DO_APP_FUNC(0x607fbf0, 0x6044870, LPVOID, GetSingletonManager, ());

DO_APP_FUNC(0x607f920, 0x60445a0, LPVOID, GetSingletonInstance, (LPVOID obj, Il2CppString* value));
