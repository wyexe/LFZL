#include "BagAttribute.h"
#include <LogLib/Log.h>
#include <Core/Feature/Searcher/ObjectSearcher.h>
#include <Core/Feature/GameMemory/GameMemory.h>

#define _SELF L"BagAttribute.cpp"
CBagAttribute::CBagAttribute()
{
}

CBagAttribute::~CBagAttribute()
{
}

UINT CBagAttribute::GetVecBagItem(_Out_ std::vector<CItem>& Vec)
{
	// dd [[[[[[[[[[[[[[[[0x013F24F0+38]+1*4]+14]-8]+1C4]+4AAC+0C]+1*4]+4]-4+4CE8+88]+4]+44]+0*4]+84]+4CE8+88]+0*10+4]+30] = ��������
	// dd [[[[[[[[[[[[[[[[0x013F24F0+38]+1*4]+14]-8]+1C4]+4AAC+0C]+1*4]+4]-4+4CE8+88]+4]+44]+0*4]+84]+4CE8+88]+0*10+4]+30] = ��������
	DWORD dwAddr = CObjectSearcher::GetGameEnv();
	return CObjectSearcher::GetVecItem(ReadDWORD(ReadDWORD(ReadDWORD(dwAddr + ��Ʒ����ƫ��1) + ��Ʒ����ƫ��2 + ��Ʒ����ƫ��3) + 0 * 0x10 + 0x4) + ��Ʒ����ƫ��4, Vec);
}

BOOL CBagAttribute::ExistItem_By_Location(_In_ CONST CItem::ItemPoint& Loc)
{
	std::vector<CItem> Vec;
	GetVecBagItem(Vec);

	return std::find_if(Vec.begin(), Vec.end(), [Loc](CONST CItem& itm) { return itm.GetItemLocation() == Loc; }) != Vec.end();
}

BOOL CBagAttribute::FindItem_By_Name(_In_ CONST std::wstring& wsItemName, _Out_ CItem& Item)
{
	std::vector<CItem> Vec;
	GetVecBagItem(Vec);


	auto itr = std::find_if(Vec.begin(), Vec.end(), [wsItemName](CONST CItem& itm) { return itm.GetName() == wsItemName; });
	if (itr != Vec.end())
	{
		Item = *itr;
		return TRUE;
	}
	return FALSE;
}

em_Equi_Color CBagAttribute::GetItemColor_By_Name(_In_ CONST std::wstring& wsItemName)
{
	CItem Item;
	if (!FindItem_By_Name(wsItemName, Item))
	{
		return em_Equi_Color::None;
	}


	return Item.GetEquiColor();
}
