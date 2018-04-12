#ifndef __LFZL_COMMON_GAMESTRUCT_H__
#define __LFZL_COMMON_GAMESTRUCT_H__

#include <ProcessLib/Memory/Memory.h>
#pragma comment(lib,"ProcessLib.lib")

#define �����ַ 0x13FB540
#define �����ַƫ��1 0x38
#define �����ַƫ��2 0x14
#define �����ַƫ��3 0x8
#define �����ַƫ��4 0x1C0
#define �����ַƫ��5 0x1DC
#define �����ַƫ��6 0x2C
#define �����ַƫ��7 0x14
#define �������ֻ�ַ 0x153F1F8
#define ����ȼ�ƫ�� 0x40
#define ��Ʒ����ƫ��1 0x1C4
#define ��Ʒ����ƫ��2 0x4B30
#define ��Ʒ����ƫ��3 0xC
#define ��Ʒ����ƫ��4 0x4D70
#define ��Ʒ����ƫ��5 0x88
#define ��Ʒ����ƫ��6 0x44
#define ��Ʒ����ƫ��7 0x84
#define ��Ʒ����ƫ��8 0x4D70
#define ��Ʒ����ƫ��9 0x88
#define ��Ʒ����ƫ��10 0x30
#define ��Ʒ��ƫ�� 0x84
#define ��Ʒ����ƫ�� 0x54
#define ��Ʒ����ȼ�ƫ�� 0x1E0
#define ��Ʒ��ɫƫ�� 0x40
#define �ֿ�ƫ��1 0x898
#define �ֿ�ƫ��2 0xA78
#define �ֿ�ҳƫ��1 0x9B4
#define �ֿ�ҳƫ��2 0xAFC
#define �ֿ�ҳ����ƫ��1 0x708
#define �ֿ�ҳ����ƫ��2 0x8E4
#define ��ǰ�ֿ�ҳƫ�� 0xB08
#define ��������ƫ�� 0x58
#define ��Χ�������ƫ��1 0x28
#define ��Χ�������ƫ��2 0x1D8
#define ��Χ�������ƫ��3 0x8E0
#define ��Χ�������ƫ��4 0x4B18


////һЩ�̶���ƫ��//////////////////////////////////////////////////////////////////////
#define �����ַƫ��8		0xC
#define ����HPƫ�� 0x2C
#define ����MAXHPƫ�� 0x2C + 0x4
#define ����MPƫ�� 0x54
#define ����MAXMPƫ�� 0x54 + 0x4
#define ���ﻤ��ƫ�� 0x78
#define ����MAX����ƫ�� 0x78 + 0x4
#define ��������ƫ�� 0xC
#define ��ƷIDƫ��   0x14
#define ��Ʒ���Ͻ����� 0x4
#define ��Ʒ���Ͻ����� 0x8
#define ��Ʒ���½����� 0xC
#define ��Ʒ���½����� 0x10
#define ��Ʒ���Ա�ƫ�� 0x0
#define ��Χ�������ƫ��5		0x230



#define ReadDWORD(x) libTools::CMemory::ReadDWORD(x)
#define ReadBYTE(x)  libTools::CMemory::ReadBYTE(x)


enum class em_Equi_Color
{
	None,
	White,
	Magic,
	Rate,
	Legend,

};

enum class em_Object_Type
{
	// ͷ��
	Helmets, 
	// ��ָ
	Ring,
	// �·�
	BodyArmours,
	// ����
	Weapons,
	// ��ʯ
	Gems,
	// �����
	Amulets,
	// ����
	Shields,
	// ����
	Gloves,
	// ����
	Belts,
	// Ь��
	Boots,
	// ҩ��
	Flasks,
	// ���˿�
	DivinationCards,
	// ��ͨ��Ʒ
	Currency,
	// ���
	Player,
	// Npc
	Npc,
	// �Npc����������Ʒ
	QuestObjects,
	// ����
	Monster,
	// ������Ʒ
	WorldItem,
	// ����
	Chests,
	// ���͵�
	Waypoint,
	// ������
	AreaTransition,
	// ����
	MiscellaneousObjects,
	//
	Other
};

struct Point
{
	DWORD X, Y;

	Point()
	{
		X = Y = NULL;
	}

	Point(_In_ DWORD dwX, _In_ DWORD dwY) : X(dwX), Y(dwY)
	{

	}

	bool operator == (CONST Point& Point_) CONST
	{
		return this->X == Point_.X && this->Y == Point_.Y;
	}

	bool operator != (CONST Point& Point_) CONST
	{
		return this->X != Point_.X || this->Y != Point_.Y;
	}

	DWORD Encode()
	{
		return X << 16 | Y;
	}

	static Point Decode(_In_ DWORD dwValue)
	{
		Point Pt;
		Pt.Y = dwValue & 0xFFFF;
		Pt.X = dwValue >> 16;
		return Pt;
	}

	BOOL IsZero() CONST
	{
		return X == 0 || Y == 0;
	}
};







#endif // !__LFZL_COMMON_GAMESTRUCT_H__
