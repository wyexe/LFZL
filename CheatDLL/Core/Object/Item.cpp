#include "Item.h"
#include "AttributeObject.h"
#include <LogLib/Log.h>
#include <Core/Feature/EchoAction/PersonAction.h>
#include <TimeLib/TimeTick.h>
#include <Core/Feature/Attribute/Bag/BagAttribute.h>
#include <Core/Feature/PointConvert/PointConverter.h>

#define _SELF L"Item.cpp"
CItem::CItem()
{
	
}

CItem::CItem(_In_ DWORD dwNodeBase)
{
	SetNodeBase(dwNodeBase);
	_emObjectType = CAttributeObject::GetObjectType(GetAttributeTablePtr());
}

CItem::~CItem()
{

}

VOID CItem::RefreshObjectAttribute()
{
	// Name
	if (!CAttributeObject::FillObjectAttribute_Base(this))
	{
		LOG_MSG_CF(L"FillObjectAttribute_Base = FALSE");
	}

	// ����
	CAttributeObject::FillObject_By_AttributeName(this, "Stack", _dwStackObject);
	
	// ҩ��
	if (GetType() == em_Object_Type::Flasks)
	{
		CAttributeObject::FillObject_By_AttributeName(this, "Charges", _dwChargesObject);
	}

	// Ʒ��
	CAttributeObject::FillObject_By_AttributeName(this, "Quality", _dwQualityObject);

	// 
	CAttributeObject::FillObject_By_AttributeName(this, "Base", _dwBaseObject);

	// 
	_IsEqui = CAttributeObject::FillObject_By_AttributeName(this, "Mods", _dwModObject);
}

CItem::ItemPoint CItem::GetItemLocation() CONST
{
	return ItemPoint{ ReadDWORD(GetNodeBase() + ��Ʒ���Ͻ�����),ReadDWORD(GetNodeBase() + ��Ʒ���Ͻ�����),ReadDWORD(GetNodeBase() + ��Ʒ���½�����),ReadDWORD(GetNodeBase() + ��Ʒ���½�����) };
}

DWORD CItem::GetPercentCharges() CONST
{
	if (_dwChargesObject == NULL)
		return 0;


	DWORD dwMaxCharges = ReadDWORD(ReadDWORD(_dwChargesObject + 0x8) + 0xC);
	if (dwMaxCharges == NULL)
	{
		return 0;
	}


	return ReadDWORD(_dwChargesObject + 0xC) * 100 / dwMaxCharges;
}

DWORD CItem::GetCount() CONST
{
	return ReadDWORD(_dwStackObject + 0xC) & 0xFF;
}

DWORD CItem::GetMaxCount() CONST
{
	return ReadDWORD(ReadDWORD(_dwStackObject + 0x8) + 0x18) & 0xFF;;
}

DWORD CItem::GetPercentCount() CONST
{
	DWORD dwMaxCount = GetMaxCount();
	if (dwMaxCount == 0)
	{
		return 0;
	}


	return GetCount() * 100 / dwMaxCount;
}

DWORD CItem::GetQuality() CONST
{
	return _dwQualityObject == 0 ? 0 : ReadBYTE(_dwQualityObject + 0xC);
}

BOOL CItem::IsBindAccount() CONST
{
	return ReadBYTE(_dwBaseObject + ��Ʒ��ƫ��) != 0;
}

BOOL CItem::IsNotAppraisal() CONST
{
	return ReadBYTE(_dwModObject + ��Ʒ����ƫ��) == 0;
}

em_Equi_Color CItem::GetEquiColor() CONST
{
	switch (ReadBYTE(_dwModObject + 0x18 + ��Ʒ��ɫƫ��))
	{
	case 0:
		return em_Equi_Color::White;
	case 1:
		return em_Equi_Color::Magic;
	case 2:
		return em_Equi_Color::Rate;
	case 3:
		return em_Equi_Color::Legend;
	default:
		break;
	}

	return em_Equi_Color::None;
}

std::wstring CItem::GetEquiColorText() CONST
{
	switch (GetEquiColor())
	{
	case em_Equi_Color::White:
		return L"��ɫ";
	case em_Equi_Color::Magic:
		return L"ħ��";
	case em_Equi_Color::Rate:
		return L"ϡ��";
	case em_Equi_Color::Legend:
		return L"����";
	default:
		break;
	}

	return L"��Ч��ɫ";
}

DWORD CItem::GetEquiLevel() CONST
{
	return ReadBYTE(_dwModObject + ��Ʒ����ȼ�ƫ�� - 0x4);
}


BOOL CItem::IsEqui() CONST
{
	return _IsEqui;
}

em_Object_Type CItem::GetType() CONST
{
	return _emObjectType;
}


CItem::em_EchoItem_Type CItem::GetEchoItemType() CONST
{
	return em_EchoItem_Type::Keep;
}

VOID CItem::ItemSelect(_In_ em_ItemLocation_Type emLocType) CONST
{
	if (emLocType == em_ItemLocation_Type::Bag)
	{
		auto ItemPos = GetItemLocation();
		Point Pos = g_BagUi_Item_StartPos;


		Pos.X += ItemPos.dwLeftTopIndex * BAG_ITEM_CELL_WIDTH + 20;
		Pos.Y += ItemPos.dwRightTopIndex * BAG_ITEM_CELL_HEIGHT + 20;
		Pos = CPointConverter::ConvertClientPosToMousePos(Pos);
		CPersonAction::MouseMove(Pos);
	}
}

VOID CItem::ItemClick(_In_ em_ItemLocation_Type emLocType) CONST
{
	libTools::CTimeTick TimeTick;
	while (!CBagAttribute::IsExistCursorItem(nullptr) && TimeTick.GetSpentTime(libTools::CTimeTick::em_TimeTick::em_TimeTick_Second) < 30)
	{
		ItemSelect(emLocType);
		CPersonAction::MouseClick();
		::Sleep(1000);
	}
	
}

VOID CItem::ItemCtrlClick(_In_ em_ItemLocation_Type emLocType) CONST
{
	ItemSelect(emLocType);
	CPersonAction::MouseCtrlClick();
	::Sleep(1000);
}

VOID CItem::ItemRightClick(_In_ em_ItemLocation_Type emLocType) CONST
{
	ItemSelect(emLocType);
	CPersonAction::MouseRightClick();
	::Sleep(1000);
}

