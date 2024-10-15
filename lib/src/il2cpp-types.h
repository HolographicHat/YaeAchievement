#pragma once
#include <cstdint>

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
};

static_assert(alignof(Array<uint8_t>) == 8, "Array alignment is incorrect");
static_assert(offsetof(Array<uint8_t>, vector) == 32, "vector offset is incorrect");

#pragma pack(push, 1)
struct PacketMeta
{
	uint16_t HeadMagic;
	uint16_t CmdId;
	uint16_t HeaderLength;
	uint32_t DataLength;
	uint8_t Data[1];
};
#pragma pack(pop)

static_assert(offsetof(PacketMeta, CmdId) == 2, "CmdId offset is incorrect");
static_assert(offsetof(PacketMeta, HeaderLength) == 4, "HeadLength offset is incorrect");
static_assert(offsetof(PacketMeta, DataLength) == 6, "DataLength offset is incorrect");
static_assert(offsetof(PacketMeta, Data) == 10, "Data offset is incorrect");