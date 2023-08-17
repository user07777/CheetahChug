#pragma once
#include <Windows.h>
#include <vector>

class MemMan
{
public:
	MemMan();
	~MemMan();
	template <class val>
	val readMem(DWORD addr)
	{
		val x;
		ReadProcessMemory(handle, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}
	template <class val>
	void writeMem(DWORD addr, val x)
	{
		WriteProcessMemory(handle, (LPBYTE*)addr, &x, sizeof(x), NULL);
	}
	DWORD getProcess(const wchar_t*);
	uintptr_t getModule(DWORD, const wchar_t*);
	DWORD getAddress(DWORD, std::vector<DWORD>);
	HANDLE handle;
};