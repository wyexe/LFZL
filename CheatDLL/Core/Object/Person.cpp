#include "Person.h"
#include "AttributeObject.h"
#include <Core/Feature/Attribute/State/StateAttribute.h>
#include <LogLib/Log.h>

#define _SELF L"Person.cpp"

CPerson::CPerson(_In_ DWORD dwNodeBase)
{
	SetNodeBase(dwNodeBase);
}

CPerson& CPerson::GetInstance()
{
	static CPerson Instance;
	return Instance;
}

VOID CPerson::RefreshObjectAttribute()
{
	SetNodeBase(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(�����ַ + �����ַƫ��1) + 1 * 4) + �����ַƫ��2) - �����ַƫ��3) + �����ַƫ��4) + �����ַƫ��5);

	// [[GetNodeBase()]+2C]+1c,20 = Current Point
	CAttributeObject::FillObject_By_AttributeName(this, "Positioned", _dwPositionedObject);
	CAttributeObject::FillObject_By_AttributeName(this, "Life", _dwLifeAttributeAddr);
	CAttributeObject::FillObject_By_AttributeName(this, "Player", _dwPlayerAttributeAddr);
	CAttributeObject::FillObjectAttribute_Player(this);
	_dwAreaLoadingStateAddr = CStateAttribute::FindState(CStateAttribute::em_State_Type::AreaLoadingState);
	if (_dwAreaLoadingStateAddr == NULL)
	{
		LOG_MSG_CF(L"FindState AreaLoadingState = NULL!");
	}

	DWORD dwPathfinding = 0;
	CAttributeObject::FillObject_By_AttributeName(this, "Pathfinding", dwPathfinding);
	LOG_C_D(L"Person._dwPositionedObject=[%X],dwPathfinding=%X", dwPathfinding);
}

DWORD CPerson::GetPercentHP() CONST
{
	return GetPercentValue(����HPƫ��);
}

DWORD CPerson::GetPercentMP() CONST
{
	return GetPercentValue(����MPƫ��);
}

DWORD CPerson::GetPercentShield() CONST
{
	return GetPercentValue(���ﻤ��ƫ��);
}

DWORD CPerson::GetLevel() CONST
{
	return ReadBYTE(_dwPlayerAttributeAddr + ����ȼ�ƫ��);
}

std::wstring CPerson::GetMapName()
{
	return CGameMemory::GetInstance().ReadProcTextWithLength(_dwAreaLoadingStateAddr + ��ǰ��ͼƫ��);
}

DWORD CPerson::GetLifeAttributeAddr() CONST
{
	return _dwLifeAttributeAddr;
}


BOOL CPerson::IsMoving() CONST
{
	return TRUE;
}

DWORD CPerson::GetPercentValue(_In_ DWORD dwOffset) CONST
{
	DWORD dwMaxValue = ReadDWORD(_dwLifeAttributeAddr + dwOffset + 0x4);
	if (dwMaxValue == 0)
	{
		return 0;
	}


	return ReadDWORD(_dwLifeAttributeAddr + dwOffset) * 100 / dwMaxValue;
}
