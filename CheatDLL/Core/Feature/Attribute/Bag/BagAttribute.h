#ifndef __LFZL_CHEATDLL_CORE_FEATURE_ATTRIBUTE_BAG_BAGATTRIBUTE_H__
#define __LFZL_CHEATDLL_CORE_FEATURE_ATTRIBUTE_BAG_BAGATTRIBUTE_H__

#include <vector>
#include <GameStruct.h>
#include <Core/Object/Item.h>


class CBagAttribute
{
public:
	CBagAttribute();
	~CBagAttribute();

	// ��ȡ��������Ʒ
	static UINT GetVecBagItem(_Out_ std::vector<CItem>& Vec);

	// �ж�������Ƿ�����Ʒ(�����ѡ��)
	static BOOL IsExistCursorItem(_Out_opt_ CItem* pItem);


	static BOOL ExistItem_By_Location(_In_ CONST CItem::ItemPoint& Loc);


	static BOOL FindItem_By_Name(_In_ CONST std::wstring& wsItemName, _Out_ CItem& Item);

	static em_Equi_Color GetItemColor_By_Name(_In_ CONST std::wstring& wsItemName);
private:

};


#endif // !__LFZL_CHEATDLL_CORE_FEATURE_ATTRIBUTE_BAG_BAGATTRIBUTE_H__
