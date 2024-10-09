using namespace Genshin;

// DO_APP_FUNC(CN_OFFSET, OS_OFFSET, RETURN, FUNC_NAME, (ARGS...));

DO_APP_FUNC(0x002ED400, 0x002ED390, ByteArray*, il2cpp_array_new_specific, (void* arrayTypeInfo, uint64_t length));

DO_APP_FUNC(0x0113ADC0, 0x0113AFC0, ByteArray*, RecordUserData, (int32_t nType));

DO_APP_FUNC(0x0F826CF0, 0x0F825F10, uint16_t, BitConverter_ToUInt16, (ByteArray* val, int startIndex));
