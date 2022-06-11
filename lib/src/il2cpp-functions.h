using namespace Genshin;

DO_APP_FUNC(0x02BC1930, ByteArray*, MessageExtensions_ToByteArray, (IMessage msg, MethodInfo* method));
DO_APP_FUNC(0x04E9B3F0, String*, Convert_ToBase64String, (ByteArray* arr, MethodInfo* method));
DO_APP_FUNC(0x01591250, void, Packet_SetDispstchData, (ByteArray* data, MethodInfo* method));
