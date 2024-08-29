using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

DO_APP_FUNC(0x00572860, 0x0056DB70, ByteArray*, il2cpp_array_new_specific, (void* arrayTypeInfo, uint64_t length));

DO_APP_FUNC(0x01663180, 0x0165E180, ByteArray*, RecordUserData, (int32_t nType));

DO_APP_FUNC(0x10986650, 0x10981A40, uint16_t, BitConverter_ToUInt16, (ByteArray* val, int startIndex));
