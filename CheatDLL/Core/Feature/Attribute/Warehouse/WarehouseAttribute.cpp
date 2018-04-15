#include "WarehouseAttribute.h"
#include <Core/Object/Item.h>
#include <Core/Feature/Searcher/ObjectSearcher.h>
#include <Core/Feature/GameMemory/GameMemory.h>

BOOL CWarehouseAttribute::IsOpenedWarehouse()
{
	DWORD dwNodeBase = GetWarehouseNodeBase();
	if (((ReadDWORD(ReadDWORD(dwNodeBase + �ֿ�ƫ��1 + 0xC) + �ֿ�ƫ��2 + 0x4) & 0x0000FF00) >> 0x8) != 0x1)
	{
		return FALSE;
	}


	return GetWarehouseObject() != NULL;
}

DWORD CWarehouseAttribute::GetWarehousePageCount()
{
	DWORD dwWarehouseObject = GetWarehouseObject();
	return (ReadDWORD(dwWarehouseObject + �ֿ�ҳƫ��1 + 0x8) - ReadDWORD(dwWarehouseObject + �ֿ�ҳƫ��1 + 0x4)) / 0xC;
}

DWORD CWarehouseAttribute::GetCurrentPageIndex()
{
	return ReadDWORD(ReadDWORD(GetWarehouseObject() + �ֿ�ҳƫ��1) + ��ǰ�ֿ�ҳƫ��) & 0xFF;
}

DWORD CWarehouseAttribute::GetCurrentPageObject()
{
	return ReadDWORD(ReadDWORD(ReadDWORD(GetWarehouseObject() + �ֿ�ҳƫ��1) + �ֿ�ҳƫ��2) + GetCurrentPageIndex() * 0x20);
}

UINT CWarehouseAttribute::GetCurrentPageItem(_Out_ std::vector<CItem>& Vec)
{
	DWORD dwWarehousePageObject = GetWarehouseObject();
	if (ReadDWORD(dwWarehousePageObject + �ֿ�ҳ����ƫ��1) == NULL)
	{
		return 0;
	}

	DWORD dwAttributeIndex = ReadDWORD(ReadDWORD(ReadDWORD(dwWarehousePageObject + �ֿ�ҳ����ƫ��1) + 0x0) + �ֿ�ҳ����ƫ��2);

	DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(�����ַ + �����ַƫ��1) + 1 * 4) + �����ַƫ��2) - �����ַƫ��3) + ��Ʒ����ƫ��1);
	dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(dwAddr + ��Ʒ����ƫ��2 + ��Ʒ����ƫ��3) + 1 * 4) + 0x4) - 4 + ��Ʒ����ƫ��4 + ��Ʒ����ƫ��5) + 0x4) + ��Ʒ����ƫ��6);
	dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(dwAddr + 0 * 4) + ��Ʒ����ƫ��7) + ��Ʒ����ƫ��8 + ��Ʒ����ƫ��9) + (dwAttributeIndex - 1) * 0x10 + 4) + ��Ʒ����ƫ��10;
	

	return CObjectSearcher::GetVecItem(dwAddr, Vec);
}

DWORD CWarehouseAttribute::GetWarehouseNodeBase()
{
	return ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(�����ַ + �����ַƫ��1) + 1 * 4) + �����ַƫ��2) - �����ַƫ��3) + ��Ʒ����ƫ��1);
}

DWORD CWarehouseAttribute::GetWarehouseObject()
{
	return ReadDWORD(ReadDWORD(GetWarehouseNodeBase() + �ֿ�ƫ��1 + 0xC) + �ֿ�ƫ��2 - 0x4);
}
