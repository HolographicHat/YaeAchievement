using namespace Genshin;

DO_APP_FUNC(0x05E24240, 0x04EA10F0, Il2CppString*, Convert_ToBase64String, (ByteArray* value, int offset, int length, void* method));
DO_APP_FUNC(0x018280A0, 0x018293F0, void, Packet_Xor, (ByteArray** data, int length, void* method));

DO_APP_FUNC(0x0193BA70, 0x0193C7D0, int, Kcp_Send, (void* client, KcpPacket* pkt, void* method));
DO_APP_FUNC(0x029EF820, 0x029F05C0, bool, Kcp_Recv, (void* client, ClientKcpEvent* evt, void* method));
