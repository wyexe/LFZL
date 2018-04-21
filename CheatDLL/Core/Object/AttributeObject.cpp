#include "AttributeObject.h"
#include <set>
#include <algorithm>
#include <CharacterLib/Character.h>
#include <LogLib/Log.h>
#pragma comment(lib,"CharacterLib.lib")

#define _SELF L"AttributeObject.cpp"
BOOL CAttributeObject::FillAttributeTable(_In_ DWORD dwAttributeNodeBase, _In_ LPCSTR pwszAttributeName, _In_ std::function<VOID(DWORD)> Ptr)
{
	DWORD dwItemAttributeTableArray = ReadDWORD(ReadDWORD(dwAttributeNodeBase + 0x0) + �����ַƫ��6) + �����ַƫ��7 + �����ַƫ��8;

	DWORD dwCount = (ReadDWORD(dwItemAttributeTableArray + 0x4) - ReadDWORD(dwItemAttributeTableArray)) / 4;
	for (DWORD i = 0;i < dwCount; ++i)
	{
		DWORD dwItemObjectAttributeTableNode = ReadDWORD(ReadDWORD(dwItemAttributeTableArray) + i * 4);
		DWORD dwItemAttributeTableObject = ReadDWORD(dwItemObjectAttributeTableNode);
		DWORD dwIndex = ReadDWORD(dwItemAttributeTableObject + 0xC);
		if (dwIndex >= 0x10)
			continue;
		else if (ReadDWORD(dwItemAttributeTableObject + 0x8) == 0)
			continue;


		std::string szItemAttributeTableObjectName = CGameMemory::GetInstance().ReadProcASCIITextWithoutLength(ReadDWORD(dwItemAttributeTableObject + 0x8));
		if (szItemAttributeTableObjectName.length() >= 16)
			continue;


		if (strcmp(pwszAttributeName, szItemAttributeTableObjectName.c_str()) == 0x0)
		{
			Ptr(dwIndex);
			return TRUE;
		}
	}
	return FALSE;
}

em_Object_Type CAttributeObject::GetObjectType(_In_ DWORD dwAttributeTablePtr)
{
	DWORD dwNodeResPtr = ReadDWORD(dwAttributeTablePtr);
	DWORD dwTextLength = ReadDWORD(dwNodeResPtr + 0x10 + 0x10);
	if (dwTextLength == 0 || dwTextLength >= 128)
	{
		LOG_C_E(L"dwAttributeTablePtr=%X, dwTextLength=%d", dwAttributeTablePtr, dwTextLength);
		return em_Object_Type::Other;
	}


	struct ObjectTypeContent
	{
		em_Object_Type emObjectType;
		std::wstring   wsText;
	};


	CONST static std::vector<ObjectTypeContent> Vec =
	{
		{ em_Object_Type::Helmets , L"Metadata/Items/Armours/Helmets" },
		{ em_Object_Type::Ring , L"Metadata/Items/Rings" },
		{ em_Object_Type::BodyArmours , L"Metadata/Items/Armours/BodyArmours" },
		{ em_Object_Type::Weapons , L"Metadata/Items/Weapons" },
		{ em_Object_Type::Gems , L"Metadata/Items/Gems" },
		{ em_Object_Type::Amulets , L"Metadata/Items/Amulets" },
		{ em_Object_Type::Shields , L"Metadata/Items/Armours/Shields" },
		{ em_Object_Type::Gloves , L"Metadata/Items/Armours/Gloves" },
		{ em_Object_Type::Belts , L"Metadata/Items/Belts" },
		{ em_Object_Type::Boots , L"Metadata/Items/Armours/Boots" },
		{ em_Object_Type::Flasks , L"Metadata/Items/Flasks" },
		{ em_Object_Type::DivinationCards , L"Metadata/Items/DivinationCards" },
		{ em_Object_Type::Currency , L"Metadata/Items/Currency" },
		{ em_Object_Type::Player , L"Metadata/Characters" },
		{ em_Object_Type::Npc , L"Metadata/NPC" },
		{ em_Object_Type::Npc , L"Metadata/MiscellaneousObjects/Stash" }, // �Ѳֿ⵱��Npc������ˡ���
		{ em_Object_Type::Npc , L"Metadata/MiscellaneousObjects/Waypoint" }, // ���͵�Ҳ����Npc����...
		{ em_Object_Type::QuestObjects , L"Metadata/QuestObjects" },
		{ em_Object_Type::Monster , L"Metadata/Monsters" },
		{ em_Object_Type::WorldItem , L"MiscellaneousObjects/WorldItem" },
		{ em_Object_Type::Chests , L"Metadata/Chests" },
		//{ em_Object_Type::Waypoint , L"MiscellaneousObjects/Waypoint" },
		{ em_Object_Type::AreaTransition , L"MiscellaneousObjects/AreaTransition" },
		{ em_Object_Type::MiscellaneousObjects , L"Metadata/MiscellaneousObjects" },
		{ em_Object_Type::Maps , L"Metadata/Items/Maps/" },
		{ em_Object_Type::MapFragment , L"Metadata/Items/MapFragments/" },
		{ em_Object_Type::Jewels , L"Metadata/Items/Jewels/" },
	};


	std::wstring wsResName = CGameMemory::GetInstance().ReadProcTextWithLength(dwNodeResPtr + 0x10);


	CONST static std::set<std::wstring> VecFilter = { L"Metadata/NPC/Blocker" };
	if (VecFilter.find(wsResName) != VecFilter.end())
	{
		return em_Object_Type::Other;
	}

	//LOG_C_D(L"wsResName=%s", wsResName.c_str());
	auto itr = std::find_if(Vec.begin(), Vec.end(), [wsResName](CONST ObjectTypeContent& itm) { return wsResName.find(itm.wsText) != std::wstring::npos; });
	return itr != Vec.end() ? itr->emObjectType : em_Object_Type::Other;
}

std::wstring CAttributeObject::GetObjectTypeText(_In_ em_Object_Type emObjectType)
{
	switch (emObjectType)
	{
	case em_Object_Type::Helmets:
		return L"ͷ��";
	case em_Object_Type::Ring:
		return L"��ָ";
	case em_Object_Type::BodyArmours:
		return L"�·�";
	case em_Object_Type::Weapons:
		return L"����";
	case em_Object_Type::Gems:
		return L"��ʯ";
	case em_Object_Type::Amulets:
		return L"�����";
	case em_Object_Type::Shields:
		return L"����";
	case em_Object_Type::Gloves:
		return L"����";
	case em_Object_Type::Belts:
		return L"����";
	case em_Object_Type::Boots:
		return L"Ь��";
	case em_Object_Type::Flasks:
		return L"ҩ��";
	case em_Object_Type::DivinationCards:
		return L"���˿�";
	case em_Object_Type::Currency:
		return L"ͨ��";
	case em_Object_Type::Player:
		return L"���";
	case em_Object_Type::Npc:
		return L"Npc";
	case em_Object_Type::QuestObjects:
		return L"����Npc";
	case em_Object_Type::Monster:
		return L"����";
	case em_Object_Type::WorldItem:
		return L"������Ʒ";
	case em_Object_Type::Chests:
		return L"����";
	case em_Object_Type::Waypoint:
		return L"���͵�";
	case em_Object_Type::AreaTransition:
		return L"������";
	case em_Object_Type::MiscellaneousObjects:
		return L"����";
	case em_Object_Type::Maps:
		return L"����ͼ";
	case em_Object_Type::MapFragment:
		return L"��ͼ��Ƭ";
	case em_Object_Type::Jewels:
		return L"�츳��ʯ";
	default:
		break;
	}

	return L"UnKnow";
}
