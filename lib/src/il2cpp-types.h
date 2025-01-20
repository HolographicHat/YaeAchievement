#pragma once
#include <cstdint>
#include <span>

#define PROPERTY_GET_CONST(type, name, funcBody) \
	type get_##name() const funcBody \
	__declspec(property(get = get_##name)) type name;

enum class PacketType : uint8_t
{
	None = 0,
	Achivement = 1,
	Inventory = 2,
	PropData = 100,
	End = 255,
};

template <typename T>
class Array
{
public:
	void* klass;
	void* monitor;
	void* bounds;
	size_t max_length;
	T vector[1];

	Array() = delete;

	T* data() {
		return vector;
	}

	std::span<T> AsSpan() {
		return { vector, max_length };
	}

	template <typename U>
	U As() {
		return reinterpret_cast<U>(vector);
	}
};

static_assert(alignof(Array<uint8_t>) == 8, "Array alignment is incorrect");
static_assert(offsetof(Array<uint8_t>, vector) == 32, "vector offset is incorrect");

#pragma pack(push, 1)
class PacketMeta
{
	uint16_t m_HeadMagic;
	uint16_t m_CmdId;
	uint16_t m_HeaderLength;
	uint32_t m_DataLength;
	uint8_t m_Data[1];
public:

	PacketMeta() = delete;

	PROPERTY_GET_CONST(uint16_t, HeadMagic, { return _byteswap_ushort(m_HeadMagic); });
	PROPERTY_GET_CONST(uint16_t, CmdId, { return _byteswap_ushort(m_CmdId); });
	PROPERTY_GET_CONST(uint16_t, HeaderLength, { return _byteswap_ushort(m_HeaderLength); });
	PROPERTY_GET_CONST(uint32_t, DataLength, { return _byteswap_ulong(m_DataLength); });

	std::span<uint8_t> AsSpan() {
		return { m_Data + HeaderLength, DataLength };
	}

	friend struct PacketMetaStaticAssertHelper;
};
#pragma pack(pop)

struct PacketMetaStaticAssertHelper
{
	static_assert(offsetof(PacketMeta, m_CmdId) == 2, "CmdId offset is incorrect");
	static_assert(offsetof(PacketMeta, m_HeaderLength) == 4, "HeadLength offset is incorrect");
	static_assert(offsetof(PacketMeta, m_DataLength) == 6, "DataLength offset is incorrect");
	static_assert(offsetof(PacketMeta, m_Data) == 10, "Data offset is incorrect");
};