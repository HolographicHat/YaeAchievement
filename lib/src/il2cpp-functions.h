using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

// N: System.Convert$ToBase64String
// L: mscorlib
// S: Ref/E8 ?? ?? ?? ?? 48 8B D8 EB 23 E8
DO_APP_FUNC(0x08D85020, 0x08C9CAB0, Il2CppString*, ToBase64String, (ByteArray* value, int offset, int length, void* method));

// N: MoleMole.MonoLoginMainPage.version$set
// L: Assembly-CSharp
// S: 84 C0 74 35 B9 52 FA 00 00 E8 ?? ?? ?? ?? 84 C0 74 27 B9 52 FA 00 00 E8 ?? ?? ?? ?? 48 85 C0 74 52 4C 8B C7 48 8B D3 48 8B C8 48 8B 5C 24 30 48 83 C4 20 5F E9
DO_APP_FUNC(0X05973470, 0x058C2DD0, void, SetVersion, (void* obj, Il2CppString* value, void* method));

// N: UnityEngine.Application$RecordUserData
// L: UnityEngine.CoreModule
DO_APP_FUNC(0x097086E0, 0x09615A20, ByteArray*, RecordUserData, (int32_t nType, void* method));

// N: MoleMole.Packet$XorEncrypt [Obfuscated]
// L: Assembly-CSharp
DO_APP_FUNC(0x05E2B210, 0x05D6A0D0, void, XorEncrypt, (ByteArray** data, int length, void* method));

// N: Kcp.KcpNative$kcp_client_send_packet [Obfuscated]
// L: Assembly-CSharp
DO_APP_FUNC(0x05F8B2A0, 0x05EC5E00, int, KcpSend, (void* client, KcpPacket* pkt, void* method));

// N: MoleMole.KcpClient$TryDequeueEvent [Obfuscated]
// L: Assembly-CSharp
// S: Ref/public static extern Int32 [A-Z]{11}\(IntPtr [A-Z]{11}, [A-Z]{11}& [A-Z]{11}\)
DO_APP_FUNC(0x05C21FA0, 0x05B67640, bool, KcpRecv, (void* client, ClientKcpEvent* evt, void* method));
