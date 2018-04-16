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
		DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(�����ַ + �����ַƫ��1) + 1 * 4) + �����ַƫ��2) - �����ַƫ��3) + ��Ʒ����ƫ��1);
		dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(dwAddr + ��Ʒ����ƫ��2 + ��Ʒ����ƫ��3) + 0 * 4) + 0x8 + ��Χ�������ƫ��1);
		dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(dwAddr + 4 * 4) - ��Χ�������ƫ��2) + ��Χ�������ƫ��3) + ��Χ�������ƫ��4) + ��Χ�������ƫ��5 + 0xC) + 0x4/*RootNode*/);

		std::queue<DWORD> VecStack;
		VecStack.push(dwAddr);
		

		while (!VecStack.empty())
		{
			DWORD dwNode = VecStack.front();
			VecStack.pop();

			DWORD dwLeftNode = ReadDWORD(dwNode + 0x0);
			DWORD dwRightNode = ReadDWORD(dwNode + 0x8);
			if (ReadBYTE(dwLeftNode + 0xD) == 0)
			{
				VecStack.push(dwLeftNode);
			}
			if (ReadBYTE(dwRightNode + 0xD) == 0x0)
			{
				VecStack.push(dwRightNode);
			}


			if (Matcher(ReadDWORD(dwNode + 0x14)))
			{
				Vec.emplace_back(ReadDWORD(dwNode + 0x14));
			}
		}

		return Vec.size();
	}
};



#endif // !__LFZL_CHEATDLL_CORE_FEATURE_SEARCHER_OBJECTSEARCHER_H__
