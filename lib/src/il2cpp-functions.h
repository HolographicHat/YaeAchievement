using namespace Genshin;

DO_APP_FUNC(0x04E9B0A0, String*, Convert_ToBase64String, (ByteArray* value, int offset, int length, void* method));
DO_APP_FUNC(0x015913C0, void, Packet_Xor, (ByteArray** data, int length, void* method));

DO_APP_FUNC(0x016F70C0, int, Kcp_Send, (void* client, KcpPacket* pkt, void* method));
DO_APP_FUNC(0x02AB08D0, bool, Kcp_Recv, (void* client, ClientKcpEvent* evt, void* method));
