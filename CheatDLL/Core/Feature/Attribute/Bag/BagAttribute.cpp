#include "BagAttribute.h"
#include <LogLib/Log.h>
#include <Core/Object/Item.h>
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
