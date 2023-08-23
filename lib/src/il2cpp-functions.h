using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

DO_APP_FUNC(0x21e4c70, 0x21bea20, void, SetVersion, (void* obj, Il2CppString* value, void* method));

DO_APP_FUNC(0x3173bb0, 0x3133c50, void, XorEncrypt, (ByteArray** data, int length, void* method));

DO_APP_FUNC(0x3d69990, 0x3d12160, bool, KcpRecv, (void* client, ClientKcpEvent* evt, void* method));

DO_APP_FUNC(0xfa24e0, 0xf994d0, VOID, ForceQuit, (LPVOID obj));

DO_APP_FUNC(0x72586e0, 0x7207680, LPVOID, GetSingletonManager, ());

DO_APP_FUNC(0x7258410, 0x72073b0, LPVOID, GetSingletonInstance, (LPVOID obj, Il2CppString* value));
