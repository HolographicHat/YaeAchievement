#pragma once
#include <Windows.h>
#include <span>

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

	bool Write(std::span<const uint8_t> data) const
	{
		return Write(data.data(), data.size());
	}

	template <typename T>
	bool Write(const T& data) const
	{
		return Write(&data, sizeof(T));
	}

};