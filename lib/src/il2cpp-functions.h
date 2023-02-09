using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

// N: System.Convert$ToBase64String
// L: mscorlib
DO_APP_FUNC(0x08F056A0, 0x08F1A0F0, Il2CppString*, ToBase64String, (ByteArray* value, int offset, int length, void* method));

// N: MoleMole.MonoLoginMainPage.version$set
// L: Assembly-CSharp
DO_APP_FUNC(0X05265A70, 0x052704C0, void, SetVersion, (void* obj, Il2CppString* value, void* method));

// N: UnityEngine.Application$RecordUserData
// L: UnityEngine.CoreModule
DO_APP_FUNC(0x09932F30, 0x09947590, ByteArray*, RecordUserData, (int32_t nType));

// N: MoleMole.Packet$XorEncrypt [Obfuscated]
// L: Assembly-CSharp
DO_APP_FUNC(0x054B3120, 0x054BD670, void, XorEncrypt, (ByteArray** data, int length, void* method)); // OHOBJBNAMJM

// N: Kcp.KcpNative$kcp_client_send_packet [Obfuscated]
// L: Assembly-CSharp
DO_APP_FUNC(0x050BB390, 0x050C5AC0, int, KcpSend, (void* client, KcpPacket* pkt, void* method)); // CKKPANHPFAP

// N: MoleMole.KcpClient$TryDequeueEvent [Obfuscated]
// L: Assembly-CSharp
DO_APP_FUNC(0x04496B50, 0x0449DBF0, bool, KcpRecv, (void* client, ClientKcpEvent* evt, void* method)); // GFFOOBHMCOJ

DO_APP_FUNC(0x09254010, 0x09269550, LPVOID, GetDefaultEncoding, ());

DO_APP_FUNC(0x092538C0, 0x09268DE0, Il2CppString*, GetString, (LPVOID encoding, LPVOID bytes));

DO_APP_FUNC(0x09252750, 0x09267C60, ByteArray*, GetBytes, (LPVOID encoding, LPVOID str));

DO_APP_FUNC(0X05069690, 0X05073B60, VOID, RequestLogin, (LPVOID obj, LPVOID token, UINT uid));

DO_APP_FUNC(0x03C751A0, 0x0500F3D0, VOID, SetChecksum, (LPVOID obj, Il2CppString* value));
