#include <Windows.h>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <ranges>
#include <unordered_set>
#include <unordered_map>
#include <future>
#include <mutex>

#include "globals.h"
#include <Zydis/Zydis.h>
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

	uintptr_t GetSection(LPCSTR name, size_t* sectionSize = nullptr)
	{
		using namespace Globals;
		if (BaseAddress == 0)
			return 0;

		const auto dosHeader = (PIMAGE_DOS_HEADER)BaseAddress;
		const auto ntHeader = (PIMAGE_NT_HEADERS)((uintptr_t)dosHeader + dosHeader->e_lfanew);
		const auto sectionHeader = IMAGE_FIRST_SECTION(ntHeader);

		for (auto i = 0; i < ntHeader->FileHeader.NumberOfSections; i++)
		{
			if (strcmp((char*)sectionHeader[i].Name, name) == 0)
			{
				if (sectionSize != nullptr) {
					*sectionSize = sectionHeader[i].Misc.VirtualSize;
				}
				return BaseAddress + sectionHeader[i].VirtualAddress;
			}
		}

		return 0;
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
			return op.type == ZYDIS_OPERAND_TYPE_MEMORY && op.mem.base == ZYDIS_REGISTER_RIP && op.mem.disp.size == 32;
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
				uint32_t destination = instr.RVA + instr.Instruction.length + instr.Operands[0].imm.value.s;
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
		size_t sectionSize;
		const auto sectionAddress = GetSection("il2cpp", &sectionSize);
		const auto sectionEnd = sectionAddress + sectionSize;

		printf("Section Address: 0x%llX\n", sectionAddress);
		printf("Section End: 0x%llX\n", sectionEnd);

		if (sectionAddress == 0)
			return; // message box?

		const auto candidates = Util::PatternScanAll(sectionAddress, sectionEnd, "56 48 83 EC 20 48 89 D0 48 89 CE 80 3D ? ? ? ? 00");
		printf("Candidates: %llu\n", candidates.size());

		std::vector<std::vector<DecodedInstruction>> candidateInstructions;
		std::ranges::transform(candidates, std::back_inserter(candidateInstructions), DecodeFunction);

		std::vector<std::vector<DecodedInstruction>> filteredInstructions;
		std::ranges::copy_if(candidateInstructions, std::back_inserter(filteredInstructions), [](const std::vector<DecodedInstruction>& instr) {
			return GetDataReferenceCount(instr) == 5 && GetCallCount(instr) == 10 && GetUniqueCallCount(instr) == 6 && GetCmpImmCount(instr) == 5;
		});

		// should have only one result
		if (filteredInstructions.size() != 1)
		{
			printf("Filtered Instructions: %llu\n", filteredInstructions.size());
			return;
		}

		const auto& instructions = filteredInstructions[0];
		printf("RVA: 0x%08X\n", instructions.front().RVA);

		// extract all the non-zero immediate values from the cmp instructions
		std::decay_t<decltype(instructions)> cmpInstructions;
		std::ranges::copy_if(instructions, std::back_inserter(cmpInstructions), [](const DecodedInstruction& instr) {
			return instr.Instruction.mnemonic == ZYDIS_MNEMONIC_CMP && instr.Operands[1].type == ZYDIS_OPERAND_TYPE_IMMEDIATE && instr.Operands[1].imm.value.u;
		});

		std::vector<uint32_t> cmdIds;
		std::ranges::transform(cmpInstructions, std::back_inserter(cmdIds), [](const DecodedInstruction& instr) {
			return instr.Operands[1].imm.value.u;
		});

		for (const auto& cmdId : cmdIds)
		{
			printf("CmdId: %u\n", cmdId);
			Globals::DynamicCmdIds.insert(cmdId);
		}


	}

	int32_t GetCallCount(uint8_t* target)
	{
		size_t sectionSize;
		const auto sectionAddress = GetSection("il2cpp", &sectionSize);
		const auto sectionEnd = sectionAddress + sectionSize;

		int32_t count = 0;

		ZydisDecoder decoder{};
		ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

		ZydisDecodedInstruction instruction{};
		ZydisDecoderContext context{};

		auto rip = (uint8_t*)sectionAddress;
		while (rip < (uint8_t*)sectionEnd)
		{
			auto status = ZydisDecoderDecodeInstruction(&decoder, &context, rip, ZYDIS_MAX_INSTRUCTION_LENGTH, &instruction);
			if (!ZYAN_SUCCESS(status))
			{
				rip += 1;
				continue;
			}

			if (instruction.mnemonic == ZYDIS_MNEMONIC_CALL)
			{
				const auto offset = *(int32_t*)(rip + 1);
				const auto destination = rip + 5 + offset;
				if (destination == target) {
					count++;
				}
			}

			rip += instruction.length;
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

	/// <summary>
	/// can be very slow to resolve on low-end machine,
	/// consider updating static offset after it is resolved in development environment
	/// </summary>
	/// <returns></returns>
	uintptr_t Resolve_BitConverter_ToUInt16()
	{
		size_t sectionSize;
		const auto sectionAddress = GetSection("il2cpp", &sectionSize);
		const auto sectionEnd = sectionAddress + sectionSize;

		printf("Section Address: 0x%llX\n", sectionAddress);
		printf("Section End: 0x%llX\n", sectionEnd);

		/*
			mov ecx, 0Fh
			call ThrowHelper.ThrowArgumentNullException
			mov ecx, 0Eh
			mov edx, 16h
			call ThrowHelper.ThrowArgumentOutOfRangeException
			mov ecx, 5
			call ThrowHelper.ThrowArgumentException
		*/
		auto candidates = Util::PatternScanAll(sectionAddress, sectionEnd, "B9 0F 00 00 00 E8 ? ? ? ? B9 0E 00 00 00 BA 16 00 00 00 E8 ? ? ? ? B9 05 00 00 00 E8 ? ? ? ?");
		printf("Candidates: %llu\n", candidates.size());

		std::vector<uintptr_t> filteredEntries;
		std::ranges::copy_if(candidates, std::back_inserter(filteredEntries), [](uintptr_t& entry) {
			entry = FindFunctionEntry(entry);
			return entry % 16 == 0;
		});

		for (const auto& entry : filteredEntries)
		{
			printf("Entry: 0x%llX\n", entry);
		}

		printf("Looking for call counts...\n");
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
			printf("Entry: 0x%llX, RVA: 0x%08llX, Count: %d\n", entry, entry - Globals::BaseAddress, count);
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

	printf("BaseAddress: 0x%llX\n", BaseAddress);
	printf("IsCNREL: %d\n", IsCNREL);
	printf("BitConverter_ToUInt16: 0x%llX\n", Offset.BitConverter_ToUInt16);
}
