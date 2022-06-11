#pragma once

#include "pch.h"

#define CALL_ORIGIN(function, ...) \
	HookManager::call(function, __func__, __VA_ARGS__)

class HookManager {
public:
	template <typename Fn>
	static void install(Fn func, Fn handler) {
		enable(func, handler);
		holderMap[reinterpret_cast<void*>(handler)] = reinterpret_cast<void*>(func);
	}

	template <typename Fn>
	[[nodiscard]] static Fn getOrigin(Fn handler, const char* callerName = nullptr) noexcept {
		if (holderMap.count(reinterpret_cast<void*>(handler)) == 0) {
			printf("Origin not found for handler: %s. Maybe racing bug.", callerName == nullptr ? "<Unknown>" : callerName);
			return nullptr;
		}
		return reinterpret_cast<Fn>(holderMap[reinterpret_cast<void*>(handler)]);
	}

	template <typename Fn>
	[[nodiscard]] static void detach(Fn handler) noexcept {
		disable(handler);
		holderMap.erase(reinterpret_cast<void*>(handler));
	}

	template <typename RType, typename... Params>
	[[nodiscard]] static RType call(RType(*handler)(Params...), const char* callerName = nullptr, Params... params) {
		auto origin = getOrigin(handler, callerName);
		if (origin != nullptr)
			return origin(params...);

		return RType();
	}

	static void detachAll() noexcept {
		for (const auto &[key, value] : holderMap) {
			disable(key);
		}
		holderMap.clear();
	}

private:
	inline static std::map<void*, void*> holderMap{};

	template <typename Fn>
	static void disable(Fn handler) {
		Fn origin = getOrigin(handler);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)origin, handler);
		DetourTransactionCommit();
	}

	template <typename Fn>
	static void enable(Fn& func, Fn handler) {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)func, handler);
		DetourTransactionCommit();
	}
};
