#include <Windows.h>
#include <print>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <ranges>
#include <unordered_set>
#include <unordered_map>
#include <future>
#include <mutex>
#include <immintrin.h>

#include "globals.h"
#include "Zydis.h"
#include "util.h"

namespace
{
	class DecodedInstruction
	{
	public:
		DecodedInstruction() = default;
		~DecodedInstruction() = default;
		DecodedInstruction(const ZydisDecodedInstruction& instruction) : Instruction(instruction) {}
		DecodedInstruction(const ZydisDecodedInstruction& instruction, ZydisDecodedOperand* operands, uint8_t operandCount) : Instruction(instruction) {
			Operands = { operands, operands + operandCount };
		}
		DecodedInstruction(const uint32_t rva, const ZydisDecodedInstruction& instruction, ZydisDecodedOperand* operands, uint8_t operandCount) : RVA(rva), Instruction(instruction) {
			Operands = { operands, operands + operandCount };
		}

		// copy constructor
		DecodedInstruction(const DecodedInstruction& other) = default;

		// move constructor
		DecodedInstruction(DecodedInstruction&& other) noexcept : RVA(other.RVA), Instruction(other.Instruction), Operands(std::move(other.Operands)) {}

		uint32_t RVA = 0;
		ZydisDecodedInstruction Instruction;
		std::vector<ZydisDecodedOperand> Operands;
	};

	std::span<uint8_t> GetSection(LPCSTR name)
	{
		using namespace Globals;
		if (BaseAddress == 0)
			return {};

		const auto dosHeader = (PIMAGE_DOS_HEADER)BaseAddress;
		const auto ntHeader = (PIMAGE_NT_HEADERS)((uintptr_t)dosHeader + dosHeader->e_lfanew);
		const auto sectionHeader = IMAGE_FIRST_SECTION(ntHeader);

		for (auto i = 0; i < ntHeader->FileHeader.NumberOfSections; i++)
		{
			if (strcmp((char*)sectionHeader[i].Name, name) == 0)
			{
				const auto sectionSize = sectionHeader[i].Misc.VirtualSize;
				const auto virtualAddress = BaseAddress + sectionHeader[i].VirtualAddress;
				return std::span(reinterpret_cast<uint8_t*>(virtualAddress), sectionSize);
			}
		}

		return {};
	}

	/// <summary>
	/// decodes all instruction until next push, ignores branching
	/// </summary>
	/// <param name="address"></param>
	/// <returns>std::vector DecodedInstruction</returns>
	std::vector<DecodedInstruction> DecodeFunction(uintptr_t address)
	{
		using namespace Globals;

		std::vector<DecodedInstruction> instructions;

		ZydisDecoder decoder{};
		ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

		ZydisDecodedInstruction instruction{};
		ZydisDecoderContext context{};
		ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT_VISIBLE]{};

		while (true)
		{
			const auto data = reinterpret_cast<uint8_t*>(address);
			auto status = ZydisDecoderDecodeInstruction(&decoder, &context, data, ZYDIS_MAX_INSTRUCTION_LENGTH, &instruction);
			if (!ZYAN_SUCCESS(status))
			{
				// for skipping jump tables
				address += 1;
				continue;
			}

			status = ZydisDecoderDecodeOperands(&decoder, &context, &instruction, operands, instruction.operand_count_visible);
			if (!ZYAN_SUCCESS(status))
			{
				// for skipping jump tables
				address += 1;
				continue;
			}

			if (instruction.mnemonic == ZYDIS_MNEMONIC_PUSH && !instructions.empty()) {
				break;
			}

			const auto rva = static_cast<uint32_t>(address - BaseAddress);
			instructions.emplace_back(rva, instruction, operands, instruction.operand_count_visible);

			address += instruction.length;
		}

		return instructions;
	}

	/// <summary>
	/// get the count of data references in the instructions (only second oprand of mov)
	/// </summary>
	/// <param name="instructions"></param>
	/// <returns></returns>
	int32_t GetDataReferenceCount(const std::vector<DecodedInstruction>& instructions)
	{
		return static_cast<int32_t>(std::ranges::count_if(instructions, [](const DecodedInstruction& instr) {
			if (instr.Instruction.mnemonic != ZYDIS_MNEMONIC_MOV)
				return false;

			if (instr.Operands.size() != 2)
				return false;

			const auto& op = instr.Operands[1];

			// access to memory, based off of rip, 32-bit displacement
			return op.type == ZYDIS_OPERAND_TYPE_MEMORY && op.mem.base == ZYDIS_REGISTER_RIP && op.mem.disp.has_displacement;
		}));
	}

	int32_t GetCallCount(const std::vector<DecodedInstruction>& instructions)
	{
		return static_cast<int32_t>(std::ranges::count_if(instructions, [](const DecodedInstruction& instr) {
			return instr.Instruction.mnemonic == ZYDIS_MNEMONIC_CALL;
		}));
	}

	int32_t GetUniqueCallCount(const std::vector<DecodedInstruction>& instructions)
	{
		std::unordered_set<uint32_t> calls;
		for (const auto& instr : instructions)
		{
			if (instr.Instruction.mnemonic == ZYDIS_MNEMONIC_CALL) {
				uint32_t destination = instr.Operands[0].imm.value.s + instr.RVA + instr.Instruction.length;
				calls.insert(destination);
			}
		}

		return static_cast<int32_t>(calls.size());
	}

	int32_t GetCmpImmCount(const std::vector<DecodedInstruction>& instructions)
	{
		return static_cast<int32_t>(std::ranges::count_if(instructions, [](const DecodedInstruction& instr) {
			return instr.Instruction.mnemonic == ZYDIS_MNEMONIC_CMP && instr.Operands[1].type == ZYDIS_OPERAND_TYPE_IMMEDIATE && instr.Operands[1].imm.value.u;
		}));
	}

	void ResolveCmdId()
	{
		const auto il2cppSection = GetSection("il2cpp");

		std::println("Section Address: 0x{:X}", reinterpret_cast<uintptr_t>(il2cppSection.data()));
		std::println("Section End: 0x{:X}", reinterpret_cast<uintptr_t>(il2cppSection.data() + il2cppSection.size()));

		if (il2cppSection.empty())
			return; // message box?

		const auto candidates = Util::PatternScanAll(il2cppSection, "56 48 83 EC 20 48 89 D0 48 89 CE 80 3D ? ? ? ? 00");
		std::println("Candidates: {}", candidates.size());

		std::vector<std::vector<DecodedInstruction>> filteredInstructions;
		std::ranges::copy_if(
			candidates | std::views::transform(DecodeFunction),
			std::back_inserter(filteredInstructions),
			[](const std::vector<DecodedInstruction>& instr) {
			return GetDataReferenceCount(instr) == 5 && GetCallCount(instr) == 10 &&
				GetUniqueCallCount(instr) == 6 && GetCmpImmCount(instr) == 5;
		});

		// should have only one result
		if (filteredInstructions.size() != 1)
		{
			std::println("Filtered Instructions: {}", filteredInstructions.size());
			return;
		}

		const auto& instructions = filteredInstructions[0];
		std::println("RVA: 0x{:08X}", instructions.front().RVA);

		// extract all the non-zero immediate values from the cmp instructions
		std::vector<uint32_t> cmdIds;
		std::ranges::for_each(instructions, [&cmdIds](const DecodedInstruction& instr) {
			if (instr.Instruction.mnemonic == ZYDIS_MNEMONIC_CMP &&
				instr.Operands[1].type == ZYDIS_OPERAND_TYPE_IMMEDIATE &&
				instr.Operands[1].imm.value.u != 0) {
				cmdIds.push_back(static_cast<uint32_t>(instr.Operands[1].imm.value.u));
			}
		});

		for (const auto& cmdId : cmdIds)
		{
			std::println("CmdId: {}", cmdId);
			Globals::DynamicCmdIds.insert(static_cast<uint16_t>(cmdId));
		}


	}

	int32_t GetCallCount(uint8_t* target)
	{
		const auto il2cppSection = GetSection("il2cpp");
		const auto sectionAddress = reinterpret_cast<uintptr_t>(il2cppSection.data());
		const auto sectionSize = il2cppSection.size();

		int32_t count = 0;
		const __m128i callOpcode = _mm_set1_epi8(0xE8);
		const size_t simdEnd = sectionSize / 16 * 16;

		for (size_t i = 0; i < simdEnd; i += 16) {
			// load 16 bytes from the current address
			const __m128i chunk = _mm_loadu_si128((__m128i*)(sectionAddress + i));

			// compare the loaded chunk with 0xE8 in all 16 bytes
			const __m128i result = _mm_cmpeq_epi8(chunk, callOpcode);

			// move the comparison results into a mask
			int mask = _mm_movemask_epi8(result);

			while (mask != 0) {
				DWORD first_match_idx = 0;
				_BitScanForward(&first_match_idx, mask); // index of the first set bit (match)

				// index of the instruction
				const size_t instruction_index = i + first_match_idx;

				const int32_t delta = *(int32_t*)(sectionAddress + instruction_index + 1);
				const uintptr_t dest = sectionAddress + instruction_index + 5 + delta;

				if (dest == (uintptr_t)target) {
					count++;
				}

				// clear the bit we just processed and continue with the next match
				mask &= ~(1 << first_match_idx);
			}
		}

		return count;
	}

	uintptr_t FindFunctionEntry(uintptr_t address) // not a correct way to find function entry
	{
		__try
		{
			while (true)
			{
				// go back to 'sub rsp' instruction
				uint32_t code = *(uint32_t*)address;
				code &= ~0xFF000000;

				if (_byteswap_ulong(code) == 0x4883EC00) { // sub rsp, ??
					return address;
				}

				address--;
			}

		}
		__except (1) {}

		return address;
	}

	uintptr_t Resolve_BitConverter_ToUInt16()
	{
		const auto il2cppSection = GetSection("il2cpp");

		std::print("Section Address: 0x{:X}", reinterpret_cast<uintptr_t>(il2cppSection.data()));
		std::println("Section End: 0x{:X}", reinterpret_cast<uintptr_t>(il2cppSection.data() + il2cppSection.size()));

		/*
			mov ecx, 0Fh
			call ThrowHelper.ThrowArgumentNullException
			mov ecx, 0Eh
			mov edx, 16h
			call ThrowHelper.ThrowArgumentOutOfRangeException
			mov ecx, 5
			call ThrowHelper.ThrowArgumentException
		*/
		auto candidates = Util::PatternScanAll(il2cppSection, "B9 0F 00 00 00 E8 ? ? ? ? B9 0E 00 00 00 BA 16 00 00 00 E8 ? ? ? ? B9 05 00 00 00 E8 ? ? ? ?");
		std::println("Candidates: {}", candidates.size());

		std::vector<uintptr_t> filteredEntries;
		std::ranges::copy_if(candidates, std::back_inserter(filteredEntries), [](uintptr_t& entry) {
			entry = FindFunctionEntry(entry);
			return entry % 16 == 0;
		});

		for (const auto& entry : filteredEntries)
		{
			std::println("Entry: 0x{:X}", entry);
		}

		std::println("Looking for call counts...");
		std::mutex mutex;
		std::unordered_map<uintptr_t, int32_t> callCounts;
		// find the call counts to candidate functions
		std::vector<std::future<void>> futures;
		std::ranges::transform(filteredEntries, std::back_inserter(futures), [&](uintptr_t entry) {
			return std::async(std::launch::async, [&](uintptr_t e) {
				const auto count = GetCallCount((uint8_t*)e);
				std::lock_guard lock(mutex);
				callCounts[e] = count;
			}, entry);
		});

		for (auto& future : futures) {
			future.get();
		}

		uintptr_t targetEntry = 0;
		for (const auto& [entry, count] : callCounts)
		{
			std::println("Entry: 0x{:X}, RVA: 0x{:08X}, Count: {}", entry, entry - Globals::BaseAddress, count);
			if (count == 5) {
				targetEntry = entry;
			}
		}

		return targetEntry;
	}

}

void InitIL2CPP()
{
	std::string buffer;
	buffer.resize(MAX_PATH);
	ZeroMemory(buffer.data(), MAX_PATH);
	const auto pathLength = GetModuleFileNameA(nullptr, buffer.data(), MAX_PATH);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
	{
		buffer.resize(pathLength);
		ZeroMemory(buffer.data(), pathLength);
		GetModuleFileNameA(nullptr, buffer.data(), pathLength);
	}
	buffer.shrink_to_fit();

	using namespace Globals;
	IsCNREL = buffer.find("YuanShen.exe") != std::string::npos;
	BaseAddress = (uintptr_t)GetModuleHandleA(nullptr);

	std::future<void> resolveFuncFuture = std::async(std::launch::async, [] {
		if (Offset.BitConverter_ToUInt16 != 0) {
			Offset.BitConverter_ToUInt16 += BaseAddress;
		}
		else {
			Offset.BitConverter_ToUInt16 = Resolve_BitConverter_ToUInt16();
		}
	});

	std::future<void> resolveCmdIdFuture = std::async(std::launch::async, [] {
		if (CmdId == 0) {
			ResolveCmdId();
		}
	});

	resolveFuncFuture.get();
	resolveCmdIdFuture.get();

	std::println("BaseAddress: 0x{:X}", BaseAddress);
	std::println("IsCNREL: {:d}", IsCNREL);
	std::println("BitConverter_ToUInt16: 0x{:X}", Offset.BitConverter_ToUInt16);
}
