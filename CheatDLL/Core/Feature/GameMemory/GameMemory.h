#ifndef __LFZL_CHEATDLL_CORE_FEATURE_GAMEMEMORY_GAMEMEMORY_H__
#define __LFZL_CHEATDLL_CORE_FEATURE_GAMEMEMORY_GAMEMEMORY_H__

#include <Windows.h>
#include <string>

class CGameMemory
{
public:
	CGameMemory() = default;
	~CGameMemory() = default;

	static CGameMemory& GetInstance();

	//
	BOOL InitializeReadMemory();

	//
	DWORD ReadProcDWORD(_In_ DWORD dwAddr);

	//
	DWORD ReadProcBYTE(_In_ DWORD dwAddr);

	//
	float ReadProcFloat(_In_ DWORD dwAddr);

	//
	std::wstring ReadProcTextWithLength(_In_ DWORD dwAddr);

	//
	std::wstring ReadProcTextWithoutLength(_In_ DWORD dwAddr);

	//
	std::string  ReadProcASCIITextWithoutLength(_In_ DWORD dwAddr);

	//
	HWND GetGameWnd() CONST;
private:
	template<typename T>
	T ReadMemory(_In_ UINT_PTR Addr)
	{
		T dwValue = 0;
		if (!::ReadProcessMemory(_hProcess, reinterpret_cast<LPCVOID>(Addr), &dwValue, sizeof(dwValue), 0))
		{
			return 0;
		}


		return dwValue;
	}
private:
	HANDLE _hProcess = NULL;
	HWND   _hWnd = NULL;
};




#endif // !__LFZL_CHEATDLL_CORE_FEATURE_GAMEMEMORY_GAMEMEMORY_H__
