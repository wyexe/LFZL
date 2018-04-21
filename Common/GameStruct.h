#ifndef __LFZL_COMMON_GAMESTRUCT_H__
#define __LFZL_COMMON_GAMESTRUCT_H__

#include <Windows.h>

#define �����ַ 0x13F9760
#define �����ַƫ��1 0x38
#define �����ַƫ��2 0x14
#define �����ַƫ��3 0x8
#define �����ַƫ��4 0x1C0
#define �����ַƫ��5 0x1DC
#define �����ַƫ��6 0x2C
#define �����ַƫ��7 0x14
#define �������ֻ�ַ 0x153D430
#define ����ȼ�ƫ�� 0x40
#define ��Ʒ����ƫ��1 0x1C4
#define ��Ʒ����ƫ��2 0x4D70
#define ��Ʒ����ƫ��3 0x88
#define ��Ʒ����ƫ��4 0x30
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
#define ��Χ�������ƫ��1 0x4B18
#define ��Χ�������ƫ��2 0x234
#define UIƫ��1		0x1E4
#define UIƫ��2		0xAA4
#define UI����ƫ��1 0x708
#define Ui��ʾƫ��	0x754
#define ����UIƫ�� 0x1978
#define ����ͼ����ƫ�� 0xB20
#define Npc���۴���ƫ�� 0xB18
#define Npc�Ի�����ƫ�� 0xB08
#define ������Ʒȷ�ϴ���ƫ�� 0xAE0
#define ����UIƫ�� 0xA88
#define ����Ҽ�ʹ����ƷUIƫ�� 0x9C4
#define ����Ƿ�ʹ����Ʒƫ�� 0x964
#define Stateƫ��1 0x24
#define StateObjectƫ�� 0x28
#define ��ǰ��ͼƫ�� 0xB6C
#define Buff����ƫ�� 0x98
#define EffectUIƫ�� 0xB68
#define Effect����ƫ�� 0x8E8
#define EffectID����ƫ�� 0x8F4


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
#define UI����ƫ��	Ui��ʾƫ�� - 0x20
#define ��Χ�������ƫ��3 0xC

#define ReadDWORD(x) CGameMemory::GetInstance().ReadProcDWORD(x)
#define ReadBYTE(x)  CGameMemory::GetInstance().ReadProcBYTE(x)
#define GameRun      TRUE
#define StopGame	 ::Sleep(1)


enum em_Equi_Color
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
	// ��ͼ
	Maps,
	// ��ͼ��Ƭ
	MapFragment,
	// �鱦(�츳��ʯ)
	Jewels,
	//
	Other
};

enum em_ItemPtrArrayIndex : DWORD
{
	em_ItemPtrArrayIndex_Bag				= 0x0,
	em_ItemPtrArrayIndex_Equi_BodyArmours	= 0x1,
	em_ItemPtrArrayIndex_Equi_Weaspon		= 0x2,
	em_ItemPtrArrayIndex_Equi_Shields		= 0x3,
	em_ItemPtrArrayIndex_Helmets			= 0x4,
	em_ItemPtrArrayIndex_Amulets			= 0x5,
	em_ItemPtrArrayIndex_Ring1				= 0x6,
	em_ItemPtrArrayIndex_Ring2				= 0x7,
	em_ItemPtrArrayIndex_Gloves				= 0x8,
	em_ItemPtrArrayIndex_Boots				= 0x9,
	em_ItemPtrArrayIndex_Belts				= 0xA,
	em_ItemPtrArrayIndex_Flasks				= 0xB,
	em_ItemPtrArrayIndex_Cursor				= 0xC, // ����������Ʒ
	em_ItemPtrArrayIndex_Jweels				= 0x17, // �츳ҳ�ϵ��鱦
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
