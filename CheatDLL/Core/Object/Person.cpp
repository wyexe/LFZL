#include "Person.h"
#include "AttributeObject.h"

CPerson& CPerson::GetInstance()
{
	static CPerson Instance;
	Instance.SetNodeBase(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(�����ַ + �����ַƫ��1) + 1 * 4) + �����ַƫ��2) - �����ַƫ��3) + �����ַƫ��4) + �����ַƫ��5));
	return Instance;
}

VOID CPerson::RefreshObjectAttribute()
{
	CAttributeObject::FillObject_By_AttributeName(this, "Positioned", _dwPositionedObject);
	CAttributeObject::FillObjectAttribute_Base(this);
	CAttributeObject::FillObject_By_AttributeName(this, "Life", _dwLifeAttributeAddr);
	CAttributeObject::FillObject_By_AttributeName(this, "Player", _dwPlayerAttributeAddr);
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

DWORD CPerson::GetPercentValue(_In_ DWORD dwOffset) CONST
{
	DWORD dwMaxValue = ReadDWORD(_dwLifeAttributeAddr + dwOffset + 0x4);
	if (dwMaxValue == 0)
	{
		return 0;
	}


	return ReadDWORD(_dwLifeAttributeAddr + dwOffset) * 100 / dwMaxValue;
}
