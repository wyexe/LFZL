#ifndef __LFZL_CHEATDLL_CORE_FEATURE_SEARCHER_OBJECTSEARCHER_H__
#define __LFZL_CHEATDLL_CORE_FEATURE_SEARCHER_OBJECTSEARCHER_H__

#include <vector>
#include <queue>
#include <functional>
#include <GameStruct.h>
#include <Core/Object/Item.h>
#include <Core/Object/Chest.h>
#include <Core/Object/Monster.h>
#include <Core/Object/Npc.h>
#include <Core/Object/Player.h>
#include <Core/Object/WorldItem.h>

class CObjectSearcher
{
public:
	CObjectSearcher();
	~CObjectSearcher();

	//
	static std::wstring GetCurrentStateText();

	//
	static DWORD GetBaseEnv();

	// ��Ϸ����
	static DWORD GetGameEnv();

	// UI����
	static DWORD GetUiEnv();

	// ��Ʒ
	static UINT GetVecItem(_In_ DWORD dwAddr, _Out_ std::vector<CItem>& Vec);

	// ����
	static UINT GetVecChest(_Out_ std::vector<CChest>& Vec);

	// Npc
	static UINT GetVecNpc(_Out_ std::vector<CNpc>& Vec);

	// ����
	static UINT GetVecMonster(_Out_ std::vector<CMonster>& Vec);

	// ���
	static UINT GetVecPlayer(_Out_ std::vector<CPlayer>& Vec);

	// ������Ʒ
	static UINT GetVecWorlditem(_Out_ std::vector<CWorldItem>& Vec);

	// ���͵�
	//static UINT GetVecWaypoint();

	// ������
	//static UINT GetVecAreaTransition();
private:
	template<typename T>
	static UINT GetVecObject(_Out_ std::vector<T>& Vec, _In_ std::function<BOOL(DWORD)> Matcher)
	{
		DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(GetGameEnv() + ��Ʒ����ƫ��1) + ��Χ�������ƫ��1) + ��Χ�������ƫ��2 + ��Χ�������ƫ��3) + 0x4/*RootNode*/);


		std::queue<DWORD> VecStack;
		VecStack.push(dwAddr);
		

		while (!VecStack.empty())
		{
			DWORD dwNode = VecStack.front();
			VecStack.pop();


			if (ReadBYTE(dwNode + 0xD) != 0)
				continue;


			if (Matcher(ReadDWORD(dwNode + 0x14)))
			{
				Vec.emplace_back(dwNode + 0x14);
			}

			VecStack.push(ReadDWORD(dwNode + 0x0));
			VecStack.push(ReadDWORD(dwNode + 0x8));
		}

		return Vec.size();
	}
};



#endif // !__LFZL_CHEATDLL_CORE_FEATURE_SEARCHER_OBJECTSEARCHER_H__
