using namespace Genshin;

DO_APP_FUNC(0x05254960, 0x052544E0, Il2CppString*, Convert_ToBase64String, (ByteArray* value, int offset, int length, void* method));
DO_APP_FUNC(0x020127B0, 0x02012D40, void, Packet_Xor, (ByteArray** data, int length, void* method));

DO_APP_FUNC(0X01AD8E40, 0x01AD9740, void, MonoLoginMainPage__set_version, (void* obj, Il2CppString* value, void* method));
DO_APP_FUNC(0x05C25AC0, 0x05C25E60, ByteArray*, Application_RecordUserData, (int32_t nType, void* method));

DO_APP_FUNC(0x015C19D0, 0x015C2150, int, Kcp_Send, (void* client, KcpPacket* pkt, void* method));
DO_APP_FUNC(0x02CF31D0, 0x02CF33A0, bool, Kcp_Recv, (void* client, ClientKcpEvent* evt, void* method));
