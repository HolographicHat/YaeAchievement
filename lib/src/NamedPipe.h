#pragma once
#include <Windows.h>
#include <span>

template <typename T>
concept IsSpan = requires(T t) {
	{ t.data() } -> std::convertible_to<const void*>;
	{ t.size() } -> std::convertible_to<std::size_t>;
	{ t.size_bytes() } -> std::convertible_to<std::size_t>;
};

class NamedPipe
{
	HANDLE m_hPipe = INVALID_HANDLE_VALUE;
public:
	NamedPipe(HANDLE hPipe) : m_hPipe(hPipe) {}
	~NamedPipe() { if (m_hPipe != INVALID_HANDLE_VALUE) CloseHandle(m_hPipe); }

	operator HANDLE() const { return m_hPipe; }
	operator bool() const { return m_hPipe != INVALID_HANDLE_VALUE && m_hPipe != nullptr; }
	NamedPipe& operator= (HANDLE hPipe) { 
		m_hPipe = hPipe; 
		return *this; 
	}

	bool Write(const void* data, size_t size) const
	{
		DWORD bytesWritten;
		if (!WriteFile(m_hPipe, data, static_cast<DWORD>(size), &bytesWritten, nullptr) || bytesWritten != size)
			return false;
		return true;
	}

	template <IsSpan T>
	bool Write(const T& data) const
	{
		return Write(data.data(), data.size_bytes());
	}

	template <typename T>
	bool Write(const T& data) const
	{
		return Write(&data, sizeof(T));
	}

};