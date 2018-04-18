#ifndef __LFZL_CHEATDLL_CORE_OBJECT_ITEM_H__
#define __LFZL_CHEATDLL_CORE_OBJECT_ITEM_H__

#include "BaseObject.h"

class CItem : public CBaseObject
{
public:
	struct ItemPoint
	{
		DWORD dwLeftTopIndex;
		DWORD dwRightTopIndex;
		DWORD dwLeftBottomIndex;
		DWORD dwRightBottomIndex;

		bool operator == (CONST ItemPoint& Pos)
		{
			return this->dwLeftTopIndex == Pos.dwLeftTopIndex &&
				this->dwLeftBottomIndex == Pos.dwLeftBottomIndex &&
				this->dwRightBottomIndex == Pos.dwRightBottomIndex &&
				this->dwRightTopIndex == Pos.dwRightTopIndex;
		}
	};

	enum class em_EchoItem_Type
	{
		Item,
		Equi,
		Drop,
		Keep
	};

	enum class em_ItemLocation_Type
	{
		Warehouse,
		Bag
	};
public:
	CItem();
	CItem(_In_ DWORD dwNodeBase);
	~CItem();

	virtual VOID RefreshObjectAttribute() override;

	// λ��
	ItemPoint	GetItemLocation() CONST;

	// ҩ��
	DWORD		GetPercentCharges() CONST;

	// ����
	DWORD		GetCount() CONST;
	DWORD		GetMaxCount() CONST;
	DWORD		GetPercentCount() CONST;

	// Ʒ��
	DWORD		GetQuality() CONST;

	// �Ƿ���˺ŵ���Ʒ
	BOOL		IsBindAccount() CONST;

	// �Ƿ�δ��������Ʒ  δ����=TRUE
	BOOL		IsNotAppraisal() CONST;

	// ��ɫ
	em_Equi_Color GetEquiColor() CONST;

	std::wstring GetEquiColorText() CONST;

	// �ȼ�
	DWORD GetEquiLevel() CONST;

	// �Ƿ�װ��
	BOOL  IsEqui() CONST;

	// 
	em_Object_Type GetType() CONST;

	//
	em_EchoItem_Type GetEchoItemType() CONST;

	//
	VOID Select(_In_ em_ItemLocation_Type emLocType) CONST;

	//
	VOID Click(_In_ em_ItemLocation_Type emLocType) CONST;

	//
	VOID CtrlClick(_In_ em_ItemLocation_Type emLocType) CONST;

	//
	VOID RightClick(_In_ em_ItemLocation_Type emLocType) CONST;
protected:
	ItemPoint	_ItemPos;
	DWORD		_dwChargesObject = NULL;
	DWORD		_dwStackObject   = NULL;
	DWORD		_dwQualityObject = NULL;
	DWORD		_dwBaseObject    = NULL;
	DWORD		_dwModObject	 = NULL;
	BOOL		_IsEqui			 = FALSE;
	em_Object_Type  _emObjectType = em_Object_Type::Other;
};


#endif // !__LFZL_CHEATDLL_CORE_OBJECT_ITEM_H__
