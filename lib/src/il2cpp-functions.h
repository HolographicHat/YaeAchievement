using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

DO_APP_FUNC(0x6790b00, 0x6839fa0, Il2CppString*, ToBase64String, (ByteArray* value, int offset, int length, void* method));

DO_APP_FUNC(0x379bb90, 0x3821a40, void, SetVersion, (void* obj, Il2CppString* value, void* method));

DO_APP_FUNC(0x7040220, 0x70f3890, ByteArray*, RecordUserData, (int32_t nType));

DO_APP_FUNC(0xf85bf0, 0xf90450, void, XorEncrypt, (ByteArray** data, int length, void* method));

DO_APP_FUNC(0x156e740, 0x158a1e0, int, KcpSend, (void* client, KcpPacket* pkt, void* method));

DO_APP_FUNC(0x2ce2040, 0x2d44790, bool, KcpRecv, (void* client, ClientKcpEvent* evt, void* method));

DO_APP_FUNC(0x6a4e700, 0x6afb5e0, LPVOID, GetDefaultEncoding, ());

DO_APP_FUNC(0x6a4dfb0, 0x6afae80, Il2CppString*, GetString, (LPVOID encoding, LPVOID bytes));

DO_APP_FUNC(0x6a4ce50, 0x6af9d50, ByteArray*, GetBytes, (LPVOID encoding, LPVOID str));

DO_APP_FUNC(0x36a1260, 0x3723bc0, VOID, RequestLogin, (LPVOID obj, LPVOID token, UINT uid));

DO_APP_FUNC(0x26506b0, 0x1b0c690, VOID, SetChecksum, (LPVOID obj, Il2CppString* value));

DO_APP_FUNC(0xf25990, 0xf2f110, VOID, ForceQuit, (LPVOID obj));

DO_APP_FUNC(0x4aac4c0, 0x4b57410, LPVOID, GetSingletonManager, ());

DO_APP_FUNC(0x4aac1f0, 0x4b57140, LPVOID, GetSingletonInstance, (LPVOID obj, Il2CppString* value));
