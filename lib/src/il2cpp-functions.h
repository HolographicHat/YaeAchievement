using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

DO_APP_FUNC(0x0052A510, 0x0052ED10, ByteArray*, il2cpp_array_new_specific, (void* arrayTypeInfo, uint64_t length));

DO_APP_FUNC(0x01688250, 0x0168CB50, ByteArray*, RecordUserData, (int32_t nType));

DO_APP_FUNC(0x0FC7D610, 0x0FCA7240, uint16_t, BitConverter_ToUInt16, (ByteArray* val, int startIndex));
