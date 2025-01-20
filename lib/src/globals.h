#pragma once
#include <Windows.h>
#include <unordered_set>
#include "NamedPipe.h"

#define PROPERTY2(type, name, cn, os) \
	type name##_cn = cn; \
	type name##_os = os; \
	type get_##name() { return Globals::IsCNREL ? name##_cn : name##_os; } \
    void set_##name(type value) { if (Globals::IsCNREL) name##_cn = value; else name##_os = value; } \
	__declspec(property(get = get_##name, put = set_##name)) type name;

namespace Globals
{
	inline HWND GameWindow = nullptr;
	inline NamedPipe MessagePipe = nullptr;
	inline bool IsCNREL = true;
	inline uintptr_t BaseAddress = 0;

	// 5.1.0 - 24082
	inline uint16_t AchievementId = 0; // use non-zero to override dynamic search
	inline std::unordered_set<uint16_t> AchievementIdSet;

	// 5.3.0 - 23233
	inline uint16_t PlayerStoreId = 0; // use non-zero to override dynamic search

	inline bool AchievementsWritten = false;
	inline bool PlayerStoreWritten = false;

	/*
	 * PROP_PLAYER_HCOIN = 10015,
     * PROP_PLAYER_WAIT_SUB_HCOIN = 10022,
     * PROP_PLAYER_SCOIN = 10016,
     * PROP_PLAYER_WAIT_SUB_SCOIN = 10023,
     * PROP_PLAYER_MCOIN = 10025,
     * PROP_PLAYER_WAIT_SUB_MCOIN = 10026,
     * PROP_PLAYER_HOME_COIN = 10042,
     * PROP_PLAYER_WAIT_SUB_HOME_COIN = 10043,
     * PROP_PLAYER_ROLE_COMBAT_COIN = 10053,
     * PROP_PLAYER_MUSIC_GAME_BOOK_COIN = 10058,
	 */
	inline std::unordered_set<int> RequiredPlayerProperties = { 10015, 10022, 10016, 10023, 10025, 10026, 10042, 10043, 10053, 10058 };

	class Offsets
	{
	public:
		PROPERTY2(uintptr_t, BitConverter_ToUInt16, 0, 0);
		//PROPERTY2(uintptr_t, BitConverter_ToUInt16, 0x0F826CF0, 0x0F825F10); // use non-zero to override dynamic search
		PROPERTY2(uintptr_t, AccountDataItem_UpdateNormalProp, 0, 0);
		//PROPERTY2(uintptr_t, AccountDataItem_UpdateNormalProp, 0x0D9FE060, 0x0D94D910); // use non-zero to override dynamic search
	};

	inline Offsets Offset;
}