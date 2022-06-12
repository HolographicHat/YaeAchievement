using namespace Genshin;

DO_APP_FUNC(0x04E9B3F0, String*, Convert_ToBase64String, (ByteArray* value, MethodInfo* method));
DO_APP_FUNC(0x0517DD30, String*, BitConverter_ToString, (ByteArray* value, INT32 start, INT32 length, MethodInfo* method));
DO_APP_FUNC(0x015913C0, void, Packet_Xor, (ByteArray** data, INT32 length, MethodInfo* method));
