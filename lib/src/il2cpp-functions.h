using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

DO_APP_FUNC(0x504620, 0x5C3140, LPVOID, il2cpp_object_new, (LPVOID t));

DO_APP_FUNC(0x05167E00, 0x05166410, ByteArray*, RecordUserData, (int32_t nType));

DO_APP_FUNC(0x04450D40, 0x0444F3B0, void, OnAchievementAllDataNotify, (LPVOID obj, LPVOID ntf));

DO_APP_FUNC(0x07A3EC40, 0x07A3DD10, int, CalculateSize, (LPVOID obj));

DO_APP_FUNC(0x07A3EB80, 0x07A3DC50, void, ProtoWriteTo, (LPVOID obj, LPVOID output));

DO_APP_FUNC(0x05BFDA50, 0x05BFCCD0, void, CodedOutputStreamInit, (LPVOID obj, LPVOID buffer, int offset, int length));

DO_UNI_FUNC(0x1051E0, 0x1051E0, ByteArray*, UnityEngine_RecordUserData, (int32_t nType));
