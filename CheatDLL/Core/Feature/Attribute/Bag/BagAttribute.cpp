#include "BagAttribute.h"
#include <Core/Object/Item.h>
#include <Core/Feature/Searcher/ObjectSearcher.h>
#include <Core/Feature/GameMemory/GameMemory.h>

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
	DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(�����ַ + �����ַƫ��1) + 1 * 4) + �����ַƫ��2) - �����ַƫ��3) + ��Ʒ����ƫ��1);
	dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(dwAddr + ��Ʒ����ƫ��2 + ��Ʒ����ƫ��3) + 1 * 4) + 0x4) - 4 + ��Ʒ����ƫ��4 + ��Ʒ����ƫ��5) + 0x4) + ��Ʒ����ƫ��6);
	dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(dwAddr + 0 * 4) + ��Ʒ����ƫ��7) + ��Ʒ����ƫ��8 + ��Ʒ����ƫ��9) + 0 * 0x10 + 4) + ��Ʒ����ƫ��10;
	
	return CObjectSearcher::GetVecItem(dwAddr, Vec);
}
