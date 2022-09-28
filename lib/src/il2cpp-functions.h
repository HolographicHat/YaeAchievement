using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

// N: System.Convert$ToBase64String
// L: mscorlib
DO_APP_FUNC(0x051CC420, 0x051CCB00, Il2CppString*, ToBase64String, (ByteArray* value, int offset, int length, void* method));

// N: MoleMole.MonoLoginMainPage.version$set
// L: Assembly-CSharp
DO_APP_FUNC(0X01E6CC80, 0x01E6CD90, void, SetVersion, (void* obj, Il2CppString* value, void* method));

// N: UnityEngine.Application$RecordUserData
// L: UnityEngine.CoreModule
DO_APP_FUNC(0x05AFD640, 0x05AFD630, ByteArray*, RecordUserData, (int32_t nType, void* method));

// N: MoleMole.Packet$XorEncrypt [Obfuscated]
// L: Assembly-CSharp
DO_APP_FUNC(0x0133C8A0, 0x0133D110, void, XorEncrypt, (ByteArray** data, int length, void* method));

// N: Kcp.KcpNative$kcp_client_send_packet [Obfuscated]
// L: Assembly-CSharp
DO_APP_FUNC(0x0094FBD0, 0x0094FCF0, int, KcpSend, (void* client, KcpPacket* pkt, void* method));

// N: MoleMole.KcpClient$TryDequeueEvent [Obfuscated]
// L: Assembly-CSharp
DO_APP_FUNC(0x026C8690, 0x026C9030, bool, KcpRecv, (void* client, ClientKcpEvent* evt, void* method));
