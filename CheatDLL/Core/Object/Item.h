#ifndef __LFZL_CPPINTERFACE_CORE_OBJECT_ITEM_H__
#define __LFZL_CPPINTERFACE_CORE_OBJECT_ITEM_H__

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


#endif // !__LFZL_CPPINTERFACE_CORE_OBJECT_ITEM_H__
