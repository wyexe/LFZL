#include "Expr.h"
#include <set>
#include <queue>
#include <LogLib/Log.h>
#include <ExceptionLib/Exception.h>
#include <CharacterLib/Character.h>
#include <ProcessLib/Memory/SearchBinary.h>
#include <ProcessLib/Memory/Memory.h>
#include <ProcessLib/Memory/SearchBinary.h>
#include <GameStruct.h>
#define _SELF L"Expr.cpp"

#pragma comment(lib,"ProcessLib.lib")
#undef ReadDWORD
#undef ReadBYTE
#define ReadDWORD(x) libTools::CMemory::ReadDWORD(x)
#define ReadBYTE(x)  libTools::CMemory::ReadBYTE(x)

std::function<VOID(CONST std::wstring&)> g_EchoExceptionMsgPtr = [] (CONST std::wstring& wsText)
{
	::MessageBoxW(NULL, wsText.c_str(), L"ExceptionError", NULL);
};
CExpr::CExpr()
{
	
}

CExpr::~CExpr()
{
}

std::vector<libTools::ExpressionFunPtr>& CExpr::GetVec()
{
	static std::vector<libTools::ExpressionFunPtr> Vec = 
	{
		{ std::bind(&CExpr::PrintPlayerAttribute, this, std::placeholders::_1), L"PrintPlayerAttribute" },
		{ std::bind(&CExpr::Help, this, std::placeholders::_1), L"Help" },
		{ std::bind(&CExpr::PrintItem, this, std::placeholders::_1), L"PrintItem" },
		{ std::bind(&CExpr::SwapAlloc, this, std::placeholders::_1), L"SwapAlloc" },
		{ std::bind(&CExpr::PrintWarehouse, this, std::placeholders::_1), L"PrintWarehouse" },
		{ std::bind(&CExpr::PrintAroundObject, this, std::placeholders::_1), L"PrintAroundObject" },
		{ std::bind(&CExpr::ScanBase, this, std::placeholders::_1), L"ScanBase" },
	};
	return Vec;
}

VOID CExpr::Release()
{

}

CExpr& CExpr::GetInstance()
{
	static CExpr Instance;
	return Instance;
}

VOID CExpr::Help(CONST std::vector<std::wstring>&)
{
	auto& Vec = GetVec();
	for (auto& itm : Vec)
	{
		LOG_C_D(L"MethodName=%s", itm.wsFunName.c_str());
	}
}

VOID CExpr::PrintPlayerAttribute(CONST std::vector<std::wstring>&)
{
	libTools::CException::InvokeAction(__FUNCTIONW__, []
	{
		DWORD dwArrayHead = ReadDWORD(�����ַ + �����ַƫ��1);
		DWORD dwObjectPtr = ReadDWORD(dwArrayHead + 1 * 4);
		DWORD dwCount = (ReadDWORD(�����ַ + �����ַƫ��1 + 0x4) - dwArrayHead) >> 2;
		LOG_C_D(L"dwCount=%d", dwCount);

		LOG_C_D(L"dwObjectPtr=%X", dwObjectPtr);
		DWORD dwNode = ReadDWORD(dwObjectPtr + �����ַƫ��2);
		LOG_C_D(L"[dwObjectPtr + %X] = %X", �����ַƫ��2, dwNode);
		if (dwNode == ReadDWORD(dwObjectPtr + �����ַƫ��2 - 4))
		{
			LOG_C_D(L"dwNode == ReadDWORD(dwObjectPtr + �����ַƫ��2 - 4)");
			return;
		}

		dwNode = ReadDWORD(ReadDWORD(ReadDWORD(dwNode - �����ַƫ��3) + �����ַƫ��4) + �����ַƫ��5);
		DWORD dwArrayNode = ReadDWORD(ReadDWORD(dwNode + 0) + �����ַƫ��6) + �����ַƫ��7 + �����ַƫ��8;
		LOG_C_D(L"dwNode=%X,dwArrayNodeAddr=%X, dwArrayNodeAddr=%X", dwNode, dwArrayNode, ReadDWORD(dwArrayNode));


		DWORD dwArrayCount = (ReadDWORD(dwArrayNode + 0x4) - ReadDWORD(dwArrayNode)) / 4;
		LOG_C_D(L"dwArrayCount=%d", dwArrayCount);

		std::set<DWORD> VecArray;
		for (DWORD j = 0; j < dwArrayCount; ++j)
		{
			DWORD dwObject = ReadDWORD(ReadDWORD(dwArrayNode) + j * 4);
			//LOG_C_D(L"dwObject=%X", dwObject);
			if (dwObject == NULL || ReadDWORD(dwObject + 0x8) == NULL)
				continue;

			VecArray.insert(dwObject);
		}

		LOG_C_D(L"VecArray.size=%d", VecArray.size());
		for (auto& itm : VecArray)
		{
			std::string wsName = reinterpret_cast<CONST CHAR*>(ReadDWORD(itm + 0x8));
			LOG_C_D(L"Addr=%X, Name=%s", itm, libTools::CCharacter::ASCIIToUnicode(wsName).c_str());
			if (wsName == "Life")
			{
				DWORD dwObject = ReadDWORD(ReadDWORD(dwNode + 0x4) + 4 * (ReadDWORD(itm + 0xC)));
				LOG_C_D(L"HP=%d,MaxHP=%d,MP=%d,MAXMP=%d,Shield=%d,MaxShiled=%d",
					ReadDWORD(dwObject + ����HPƫ��),
					ReadDWORD(dwObject + ����MAXHPƫ��),
					ReadDWORD(dwObject + ����MPƫ��),
					ReadDWORD(dwObject + ����MAXMPƫ��),
					ReadDWORD(dwObject + ���ﻤ��ƫ��),
					ReadDWORD(dwObject + ����MAX����ƫ��));

				LOG_C_D(L"Player.Name=%s", �������ֻ�ַ + ��������ƫ��);
			}
			else if (wsName == "Player")
			{
				DWORD dwObject = ReadDWORD(ReadDWORD(dwNode + 0x4) + 4 * (ReadDWORD(itm + 0xC)));
				LOG_C_D(L"Level=%d", ReadDWORD(dwObject + ����ȼ�ƫ��));
			}
		}
	});
}

enum
{
	em_ItemObjectIndex_RenderItem = 0x3, // ��Ⱦ
	em_ItemObjectIndex_Stack      = 0x2, // ����
	em_ItemObjectIndex_Base		  = 0x0, // ��Ʒ����
	em_ItemObjectIndex_Usable	  = 0x1, // �Ƿ����ʹ��
	em_ItemObjectIndex_Charges    = 0x3, // ҩ���ĳ���
};
/*
[��Ʒ��Դ����+10].Contain 
"Metadata/Items/Helmets" = ͷ��
"Metadata/Items/Ring"=��ָ,
"Metadata/Items/BodyArmours"=�·�,
"Metadata/Items/Weapons/OneHandWeapon"=�������� ,
"Metadata/Items/Weapons/TwoHandWeapon"=˫������ ,
"Metadata/Items/Gems"=��ʯ,
"Metadata/Items/Amulets"=�����,
"Metadata/Items/Shields"=����,
"Metadata/Items/Gloves"=����,
"Metadata/Items/Belts"=����,
"Metadata/Items/Boots"=Ь��
"Metadata/Items/Flasks"=ҩ��
"Metadata/Items/DivinationCards"=���˿�
*/

struct ItemAttributeTableContent
{
	DWORD dwIndex = 0xFF;
	std::string wsIndexName;

	ItemAttributeTableContent() {};
	ItemAttributeTableContent(_In_ DWORD Index, _In_ LPCSTR pszIndexName) : dwIndex(Index), wsIndexName(pszIndexName) {}
};

VOID GetItemAttributeTable(_In_ DWORD dwItemAttributePtr, _Out_ std::vector<ItemAttributeTableContent>& Vec)
{
	DWORD dwItemAttributeTableArray = ReadDWORD(ReadDWORD(dwItemAttributePtr + 0x0) + �����ַƫ��6) + �����ַƫ��7 + �����ַƫ��8;

	std::set<DWORD> VecIndex;
	for (DWORD dwItemObjectAttributeTableNode = ReadDWORD(dwItemAttributeTableArray); dwItemObjectAttributeTableNode < ReadDWORD(dwItemAttributeTableArray + 0x4); dwItemObjectAttributeTableNode += 4)
	{
		DWORD dwItemAttributeTableObject = ReadDWORD(dwItemObjectAttributeTableNode);
		DWORD dwIndex = ReadDWORD(dwItemAttributeTableObject + 0xC);
		if (dwIndex >= 0x10)
			continue;
		else if (ReadDWORD(dwItemAttributeTableObject + 0x8) == 0)
			continue;
		else if(VecIndex.find(dwIndex) != VecIndex.end())
			continue;

		CHAR* pszItemAttributeTableObjectName = reinterpret_cast<CHAR*>(ReadDWORD(dwItemAttributeTableObject + 0x8));
		if (strlen(pszItemAttributeTableObjectName) >= 16)
			continue;


		VecIndex.insert(dwIndex);
		if (strcmp(pszItemAttributeTableObjectName, "Base") == 0x0)
		{
			Vec.emplace_back(dwIndex, pszItemAttributeTableObjectName);
		}
		else if (strcmp(pszItemAttributeTableObjectName, "Stack") == 0x0)
		{
			Vec.emplace_back(dwIndex, pszItemAttributeTableObjectName);
		}
		else if (strcmp(pszItemAttributeTableObjectName, "Charges") == 0x0)
		{
			Vec.emplace_back(dwIndex, pszItemAttributeTableObjectName);
		}
		else if (strcmp(pszItemAttributeTableObjectName, "Mods") == 0x0)
		{
			Vec.emplace_back(dwIndex, pszItemAttributeTableObjectName);
		}
		else if (strcmp(pszItemAttributeTableObjectName, "Positioned") == 0x0)
		{
			Vec.emplace_back(dwIndex, pszItemAttributeTableObjectName);
		}
		else if (strcmp(pszItemAttributeTableObjectName, "Life") == 0x0)
		{
			Vec.emplace_back(dwIndex, pszItemAttributeTableObjectName);
		}
		else if (strcmp(pszItemAttributeTableObjectName, "Player") == 0x0)
		{
			Vec.emplace_back(dwIndex, pszItemAttributeTableObjectName);
		}
		else if (strcmp(pszItemAttributeTableObjectName, "Render") == 0x0)
		{
			Vec.emplace_back(dwIndex, pszItemAttributeTableObjectName);
		}
		else if (strcmp(pszItemAttributeTableObjectName, "Chest") == 0x0)
		{
			Vec.emplace_back(dwIndex, pszItemAttributeTableObjectName);
		}
		else if (strcmp(pszItemAttributeTableObjectName, "ObjectMagicProperties") == 0x0)
		{
			Vec.emplace_back(dwIndex, pszItemAttributeTableObjectName);
		}
		else if (strcmp(pszItemAttributeTableObjectName, "Actor") == 0x0)
		{
			Vec.emplace_back(dwIndex, pszItemAttributeTableObjectName);
		}
		else if (strcmp(pszItemAttributeTableObjectName, "WorldItem") == 0x0)
		{
			Vec.emplace_back(dwIndex, pszItemAttributeTableObjectName);
		}
	}
}

VOID PrintItem_By_Object(_In_ DWORD dwArrayHead)
{
	LOG_C_D(L"dwArrayHead=%X", dwArrayHead);
	DWORD dwCount = (ReadDWORD(dwArrayHead + 0x4) - ReadDWORD(dwArrayHead)) / 4;
	dwArrayHead = ReadDWORD(dwArrayHead);

	struct ItemObjectContent
	{
		DWORD dwItemObject;
		DWORD dwTopLeftX;
		DWORD dwTopRightY;
	};

	LOG_C_D(L"dwCount=%d", dwCount);
	std::set<DWORD> VecItemObject;
	for (DWORD i = 0; i < dwCount; ++i)
	{
		DWORD dwItemObject = ReadDWORD(dwArrayHead + i * 4);
		if (dwItemObject == NULL)
			continue;

		DWORD dwItemId = ReadDWORD(dwItemObject + ��ƷIDƫ��);
		if (dwItemId == NULL)
			continue;

		DWORD dwTopLeftX = ReadDWORD(dwItemObject + ��Ʒ���Ͻ�����);
		DWORD dwTopRightY = ReadDWORD(dwItemObject + ��Ʒ���Ͻ�����);
		DWORD dwRepeatFlag = (dwTopLeftX << 0x8) | (dwTopRightY << 0xF);
		if (VecItemObject.find(dwRepeatFlag) != VecItemObject.end())
			continue;


		VecItemObject.insert(dwRepeatFlag);
		DWORD dwBottomLeftX = ReadDWORD(dwItemObject + ��Ʒ���½�����);
		DWORD dwBottomRightY = ReadDWORD(dwItemObject + ��Ʒ���½�����);
		DWORD dwItemAttributePtr = ReadDWORD(dwItemObject + ��Ʒ���Ա�ƫ��);

		LOG_C_D(L"dwItemObject=%X,ID=%X, Point=[%d,%d,%d,%d]", dwItemObject, dwItemId, dwTopLeftX, dwTopRightY, dwBottomLeftX, dwBottomRightY);


		std::vector<ItemAttributeTableContent> VecItemAttributeTable;
		GetItemAttributeTable(dwItemAttributePtr, VecItemAttributeTable);
		for (auto& itm : VecItemAttributeTable)
		{
			if (itm.wsIndexName == "Base")
			{
				DWORD dwItemObjectBasePtr = ReadDWORD(ReadDWORD(dwItemAttributePtr + 0x4) + itm.dwIndex * 4);
				DWORD dwNamePtr = ReadDWORD(dwItemObjectBasePtr + 0x8);
				if (dwNamePtr == NULL)
					continue;

				WCHAR* wszItemNamePtr = reinterpret_cast<WCHAR*>(ReadDWORD(dwNamePtr + 0x10 + 0x14) >= 0xF ? ReadDWORD(dwNamePtr + 0x10) : (dwNamePtr + 0x10));
				LOG_C_D(L"ItemName=%s,�Ƿ��=%d", wszItemNamePtr, ReadDWORD(dwItemObjectBasePtr + ��Ʒ��ƫ��) & 0xFF);
			}
			else if (itm.wsIndexName == "Stack")
			{
				DWORD dwItemObjectStackPtr = ReadDWORD(ReadDWORD(dwItemAttributePtr + 0x4) + itm.dwIndex * 4);
				LOG_C_D(L"��ǰ��Ʒ����=%d/%d", ReadDWORD(dwItemObjectStackPtr + 0xC) & 0xFF, ReadDWORD(ReadDWORD(dwItemObjectStackPtr + 0x8) + 0x18) & 0xFF);
			}
			else if (itm.wsIndexName == "Charges")
			{
				DWORD dwItemObjectChargesPtr = ReadDWORD(ReadDWORD(dwItemAttributePtr + 0x4) + itm.dwIndex * 4);
				LOG_C_D(L"��ǰҩ�����ܴ���=%d, ����=%d", ReadDWORD(dwItemObjectChargesPtr + 0xC), ReadDWORD(ReadDWORD(dwItemObjectChargesPtr + 0x8) + 0xC));
			}
			else if (itm.wsIndexName == "Quality")
			{
				DWORD dwItemObjectQualityPtr = ReadDWORD(ReadDWORD(dwItemAttributePtr + 0x4) + itm.dwIndex * 4);
				LOG_C_D(L"��ǰƷ��=%d", ReadDWORD(dwItemObjectQualityPtr + 0xC) & 0xFF);
			}
			else if (itm.wsIndexName == "Mods")
			{
				DWORD dwItemObjectModsPtr = ReadDWORD(ReadDWORD(dwItemAttributePtr + 0x4) + itm.dwIndex * 4);

				if ((ReadDWORD(dwItemObjectModsPtr + ��Ʒ����ƫ��) & 0xFF) == 0)
				{
					LOG_C_D(L"δ��������Ʒ,dwItemObjectModsPtr=%X", dwItemObjectModsPtr);
				}

				switch (ReadBYTE(dwItemObjectModsPtr + 0x18 + ��Ʒ��ɫƫ��))
				{
				case 0:
					LOG_C_D(L"��Ʒ��ɫ=�װ�");
					break;
				case 1:
					LOG_C_D(L"��Ʒ��ɫ=ħ��");
					break;
				case 2:
					LOG_C_D(L"��Ʒ��ɫ=ϡ��");
					break;
				case 3:
					LOG_C_D(L"��Ʒ��ɫ=����");
					break;
				default:
					LOG_C_D(L"δ֪����Ʒ��ɫ=%d", ReadBYTE(dwItemObjectModsPtr + 0x18 + ��Ʒ��ɫƫ��));
					break;
				}

				LOG_C_D(L"װ���ĵȼ�=%d, ��Ҫװ���ϵĵȼ�=%d", ReadDWORD(dwItemObjectModsPtr + ��Ʒ����ȼ�ƫ�� - 0x4), ReadDWORD(dwItemObjectModsPtr + ��Ʒ����ȼ�ƫ��));
			}
		}
	}
}

VOID CExpr::PrintItem(CONST std::vector<std::wstring>&)
{
	/*if (Vec.size() == 0)
	{
		LOG_C_D(L"EmptyParam");
		return;
	}

	DWORD dwOffset = std::stoi(Vec.at(0), nullptr, 16);
	LOG_C_D(L"dwOffset=%X", dwOffset);

	using GetArrayLenPtr = DWORD(WINAPI*)();
	struct tagItemObjectVt
	{
		GetArrayLenPtr	GetLenPtr;
	};
	struct tagItemObject
	{
		tagItemObjectVt* pVt;
		DWORD			dwObject;
	};

	DWORD dwArrayHead = ReadDWORD(ReadDWORD(0x156C49C) + 0x100 + 0x4);

	tagItemObject* pNode = reinterpret_cast<tagItemObject *>(dwArrayHead);
	DWORD dwLen = pNode->pVt->GetLenPtr();
	LOG_C_D(L"dwArrayHead=%X, dwLen=%d", dwArrayHead, dwLen);

	for (DWORD dwNode = dwArrayHead + dwLen; ReadDWORD(dwNode) != NULL; dwNode += dwLen)
	{
		DWORD dwObject = reinterpret_cast<tagItemObject *>(dwNode)->dwObject;
		if (ReadDWORD(dwNode) == 0x0121B584 && ReadDWORD(dwObject + 0x82C) < 8)
		{
			if (ReadDWORD(dwObject + 0x8F8) != NULL && ReadDWORD(dwObject + 0x8F4) != NULL/ *����Object* /)
			{
				//DWORD dwItemResObject = ReadDWORD(ReadDWORD(dwObject + 0x8F8) + 0x0);
				DWORD dwItemAttributeObject = ReadDWORD(ReadDWORD(dwObject + 0x8F8) + 0x4);
				DWORD dwNamePtr = ReadDWORD(ReadDWORD(dwItemAttributeObject + 4 * 0/ *Base* /) + 0x8);
				if (dwNamePtr != NULL && ReadDWORD(dwNamePtr) != NULL && ReadDWORD(dwNamePtr + 0x10) != NULL && ReadDWORD(dwNamePtr + 0x10 + 0x10) < 32)
				{
					std::wstring wsItemName = ReadDWORD(dwNamePtr + 0x10 + 0x14) >= 0xF ? reinterpret_cast<WCHAR*>(ReadDWORD(dwNamePtr + 0x10)) : reinterpret_cast<WCHAR*>(dwNamePtr + 0x10);
					DWORD dwItemCount = wsItemName.length() >= 2 && *wsItemName.rbegin() == L'��' && (*(wsItemName.rbegin() + 1)) == L'ҩ' ? ReadDWORD(ReadDWORD(dwItemAttributeObject + em_ItemObjectIndex_Charges * 4) + 0xC) : ReadDWORD(ReadDWORD(dwItemAttributeObject + em_ItemObjectIndex_Stack * 4) + 0xC);
					dwItemCount = dwItemCount > 40 ? 0 : dwItemCount;
					LOG_C_D(L"Addr=%X, Name=%s, Count=%d, +%X=%X", dwObject, wsItemName.c_str(), dwItemCount, dwOffset, ReadDWORD(dwObject + dwOffset));
				}
			}
		}


		dwLen = reinterpret_cast<tagItemObject *>(dwNode)->pVt->GetLenPtr();
	}*/

	enum class em_Item_Type
	{
		em_Item_Type_Other,
		em_Item_Type_Currency, // ��ͨ��Ʒ
		em_Item_Type_Weapons,  // ����
		em_Item_Type_Armours,  // ����
		em_Item_Type_Belts,    // ����
		em_Item_Type_Rings,    // ��ָ
		em_Item_Type_Amulets,  // �����
		em_Item_Type_Boots,	   // Ь��
		em_Item_Type_Gloves,   // ����
		em_Item_Type_Shields,  // ����
		em_Item_Type_Gems,     // ���ܱ�ʯ
		em_Item_Type_Flaks,    // ҩ��
		em_Item_Type_DivinationCards, // ���˿�

	};
	/*
	[��Ʒ��Դ����+10].Contain "/Helmets/" = ͷ��,"/Ring/"=��ָ,"/BodyArmours/"=�·�,"/Weapons/OneHandWeapon/"=�������� ,"/Weapons/TwoHandWeapon/"=˫������ ,?
	"/Gems/"=��ʯ,"/Amulets/"=�����,"/Shields/"=����,"/Gloves/"=����,"/Belts/"=����,"/Boots/"=Ь��
	"/Flasks/"=ҩ��
	*/

	libTools::CException::InvokeAction(__FUNCTIONW__, [] 
	{
		// dd [[[[[[[[[[[[[[[[0x013F24F0+38]+1*4]+14]-8]+1C4]+4AAC+0C]+1*4]+4]-4+4CE8+88]+4]+44]+0*4]+84]+4CE8+88]+0*10+4]+30] = ��������
		// dd [[[[[[[[[[[[[[[[0x013F24F0+38]+1*4]+14]-8]+1C4]+4AAC+0C]+1*4]+4]-4+4CE8+88]+4]+44]+0*4]+84]+4CE8+88]+0*10+4]+30] = ��������
		DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(�����ַ + �����ַƫ��1) + 1 * 4) + �����ַƫ��2) - �����ַƫ��3);
		LOG_C_D(L"dwAddr=%X", dwAddr);
		dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(dwAddr + ��Ʒ����ƫ��1) + ��Ʒ����ƫ��2 + ��Ʒ����ƫ��3) + 0 * 0x10 + 0x4) + ��Ʒ����ƫ��4;
		LOG_C_D(L"dwAddr=%X", dwAddr);
		PrintItem_By_Object(dwAddr);
	});
	
}

using RtlAllocateHeapPtr =
PVOID(WINAPI*)(_In_ PVOID  HeapHandle, _In_opt_ ULONG  Flags, _In_  SIZE_T Size);

RtlAllocateHeapPtr OldRtlAllocateHeapPtr = nullptr;
PVOID WINAPI NewRtlAllocateHeap(_In_ PVOID  HeapHandle, _In_opt_ ULONG  Flags, _In_  SIZE_T Size)
{
	PVOID pAddr = OldRtlAllocateHeapPtr(HeapHandle, Flags, Size);
	if (pAddr != nullptr)
	{
		memset(pAddr, 0, Size);
	}
	return pAddr;
}

VOID CExpr::SwapAlloc(CONST std::vector<std::wstring>&)
{
	DWORD dwAddr = 0x10F719C;
	OldRtlAllocateHeapPtr = (RtlAllocateHeapPtr)ReadDWORD(dwAddr);
	LOG_C_D(L"NewRtlAllocateHeap=%X", NewRtlAllocateHeap);
	//::MessageBoxW(NULL, L"", L"", NULL);
	libTools::CMemory::WriteDWORD(dwAddr, (DWORD)NewRtlAllocateHeap);
}

VOID CExpr::PrintWarehouse(CONST std::vector<std::wstring>&)
{
	libTools::CException::InvokeAction(__FUNCTIONW__, [] 
	{
		DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(�����ַ + �����ַƫ��1) + 1 * 4) + �����ַƫ��2) - �����ַƫ��3) + ��Ʒ����ƫ��1);
		DWORD dwIsShow = ReadDWORD(ReadDWORD(dwAddr + �ֿ�ƫ��1 + 0xC) + �ֿ�ƫ��2 + 0x4);
		LOG_C_D(L"dwAddr=%X, dwIsShow=%X", dwAddr, dwIsShow);
		/*
			dd [[[[[[0x013F24F0+38]+1*4]+14]-8]+1C4]+894+0C]+0A78+4 = dwValue
			dwValue & 0xFFFF = 0x01?? ? �򿪹��򴰿�
			dwValue & 0xFF = 01 ? �򿪲ֿ� | �򿪴��͵�
		*/
		if (((dwIsShow & 0x0000FF00) >> 0x8) != 0x1)
		{
			LOG_C_D(L"��û�򿪲ֿ�");
			return;
		}

		DWORD dwWarehouseObject = ReadDWORD(ReadDWORD(dwAddr + �ֿ�ƫ��1 + 0xC) + �ֿ�ƫ��2 - 0x4);
		if (dwWarehouseObject == NULL)
		{
			LOG_C_D(L"��û�򿪲ֿ�");
			return;
		}


		DWORD dwPageCount = (ReadDWORD(dwWarehouseObject + �ֿ�ҳƫ��1 + 0x8) - ReadDWORD(dwWarehouseObject + �ֿ�ҳƫ��1 + 0x4)) / 0xC;
		LOG_C_D(L"dwPageCount=%d", dwPageCount);


		DWORD dwCurrentPageIndex = ReadDWORD(ReadDWORD(dwWarehouseObject + �ֿ�ҳƫ��1) + ��ǰ�ֿ�ҳƫ��) & 0xFF;
		LOG_C_D(L"��ǰҳ=%d", dwCurrentPageIndex);

		DWORD dwWarehousePageObject = ReadDWORD(ReadDWORD(ReadDWORD(dwWarehouseObject + �ֿ�ҳƫ��1) + �ֿ�ҳƫ��2) + dwCurrentPageIndex * 0x20);
		if (ReadDWORD(dwWarehousePageObject + �ֿ�ҳ����ƫ��1) == NULL)
		{
			LOG_C_D(L"�ֿ�ҳ[%d]��û��!!!", dwCurrentPageIndex);
			return;
		}

		DWORD dwAttributeIndex = ReadDWORD(ReadDWORD(ReadDWORD(dwWarehousePageObject + �ֿ�ҳ����ƫ��1) + 0x0) + �ֿ�ҳ����ƫ��2);
		LOG_C_D(L"dwAttributeIndex=%X", dwAttributeIndex);
		dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(�����ַ + �����ַƫ��1) + 1 * 4) + �����ַƫ��2) - �����ַƫ��3);
		LOG_C_D(L"dwAddr=%X", dwAddr);
		dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(dwAddr + ��Ʒ����ƫ��1) + ��Ʒ����ƫ��2 + ��Ʒ����ƫ��3) + (dwAttributeIndex - 0x1) * 0x10 + 0x4) + ��Ʒ����ƫ��4;
		LOG_C_D(L"dwAddr=%X", dwAddr);
		PrintItem_By_Object(dwAddr);
	});
}

VOID CExpr::PrintAroundObject(CONST std::vector<std::wstring>&)
{
	libTools::CException::InvokeAction(__FUNCTIONW__, [] 
	{
		DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(�����ַ + �����ַƫ��1) + 1 * 4) + �����ַƫ��2) - �����ַƫ��3) + ��Ʒ����ƫ��1);
		LOG_C_D(L"dwAddr=%X", dwAddr);
		dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(dwAddr + ��Ʒ����ƫ��2 + ��Ʒ����ƫ��3) + 0 * 4) + 0x8 + ��Χ�������ƫ��1);
		dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(dwAddr + 4 * 4) - ��Χ�������ƫ��2) + ��Χ�������ƫ��3) + ��Χ�������ƫ��4) + ��Χ�������ƫ��5 + 0xC) + 0x4/*RootNode*/);
		LOG_C_D(L"Root=%X", dwAddr);

		std::queue<DWORD> VecStack;
		VecStack.push(dwAddr);
		while (!VecStack.empty())
		{
			DWORD dwNode = VecStack.front();
			VecStack.pop();

			DWORD dwLeftNode = ReadDWORD(dwNode + 0x0);
			DWORD dwRightNode = ReadDWORD(dwNode + 0x8);
			if (ReadBYTE(dwLeftNode + 0xD) == 0)
			{
				VecStack.push(dwLeftNode);
			}
			if (ReadBYTE(dwRightNode + 0xD) == 0x0)
			{
				VecStack.push(dwRightNode);
			}

			// BYTE(dwNode + 0xC) == 1 ? �ɴ򿪵ı���
			// dwNode + 0x10 = ID


			DWORD dwNodeAttributePtr = ReadDWORD(dwNode + 0x14);
			// NpcWindowPtr = ReadDWORD(dwNodeAttributePtr + 0x10)
			DWORD dwNodeResPtr = ReadDWORD(dwNodeAttributePtr + 0x0);

			if (ReadDWORD(dwNodeResPtr + 0x10 + 0x10) != 0)
			{
				CONST WCHAR* pwszResName = reinterpret_cast<CONST WCHAR*>(ReadDWORD(dwNodeResPtr + 0x10 + 0x10 + 0x4) > 0x7 ? ReadDWORD(dwNodeResPtr + 0x10) : (dwNodeResPtr + 0x10));
				LOG_C_D(L"pwszResName=%s", pwszResName);
			}


			std::vector<ItemAttributeTableContent> VecItemAttributeTable;
			GetItemAttributeTable(dwNodeAttributePtr, VecItemAttributeTable);
			for (auto& itm : VecItemAttributeTable)
			{
				DWORD dwIndexObject = ReadDWORD(ReadDWORD(dwNodeAttributePtr + 0x4) + itm.dwIndex * 4);
				LOG_C_D(L"dwIndexObject=%X", dwIndexObject);
				if (itm.wsIndexName == "Positioned")
				{
					DWORD dwClientPointX = ReadDWORD(dwIndexObject + 0x10);
					DWORD dwClientPointY = ReadDWORD(dwIndexObject + 0x14);

					std::wstring wsObjectLife;
					DWORD dwLifeValue = ReadDWORD(dwIndexObject + 0x18);
					if (dwLifeValue == 0x7FFFFFFF)
						wsObjectLife = L"����";
					else if (dwLifeValue == 0x27)
						wsObjectLife = L"���������";
					else
						wsObjectLife = L"���";

					LOG_C_D(L"Point[%d,%d], dwLifeValue=[%08X,%s]", dwClientPointX, dwClientPointY, dwLifeValue, wsObjectLife.c_str());
				}
				else if (itm.wsIndexName == "Life")
				{
					LOG_C_D(L"HP=%d/%d MP=%d/%d", ReadDWORD(dwIndexObject + 0x2C), ReadDWORD(dwIndexObject + 0x30), ReadDWORD(dwIndexObject + 0x50), ReadDWORD(dwIndexObject + 0x54));
				}
				else if (itm.wsIndexName == "Player")
				{
					CONST WCHAR* pwszPlayerName = reinterpret_cast<CONST WCHAR*>(ReadDWORD(dwIndexObject + 0x10 + 0x10 + 0x4) > 0x7 ? ReadDWORD(dwIndexObject + 0x10) : (dwIndexObject + 0x10));
					DWORD dwLevel = ReadDWORD(dwIndexObject + 0x40);
					LOG_C_D(L"pwszPlayerName=%s, Level=%d", pwszPlayerName, dwLevel);
				}
				else if (itm.wsIndexName == "Render")
				{
					if (ReadDWORD(dwIndexObject + ��������ƫ�� + 0x10) != 0)
					{
						CONST WCHAR* pwszNpcName = reinterpret_cast<CONST WCHAR*>(ReadDWORD(dwIndexObject + ��������ƫ�� + 0x10 + 0x4) > 0x7 ? ReadDWORD(dwIndexObject + ��������ƫ��) : (dwIndexObject + ��������ƫ��));
						LOG_C_D(L"pwszNpcName=%s", pwszNpcName);
					}
				}
				else if (itm.wsIndexName == "Chest")
				{
					LOG_C_D(L"dwNode=%X,AttributeAddr=%X, ���Ӵ�״̬=%d", dwNode, dwNodeAttributePtr, ReadBYTE(dwIndexObject + 0x20));
				}
				/*else if (itm.wsIndexName == "ObjectMagicProperties")
				{
					enum em_MonsterColor
					{
						White = 0x0,
						Blue = 0x1,
						Gold = 0x2,
						DarkGold = 0x3
					};

					em_MonsterColor emColor = (em_MonsterColor)ReadDWORD(dwIndexObject + 0x50);

					
					DWORD dwNameCount = (ReadDWORD(dwIndexObject + 0x14) - ReadDWORD(dwIndexObject + 0x10)) / 0x8;
					if (dwNameCount > 3 || dwNameCount == 0)
					{
						LOG_C_D(L"Invalid dwNameCount=%d", dwNameCount);
						continue;
					}

					std::wstring wsMonsterName;
					for (DWORD i = 0;i < dwNameCount; ++i)
					{
						DWORD dwMonsterNamePtr = ReadDWORD(dwIndexObject + 0x10) + i * 0x8 + 0x4;
						wsMonsterName += reinterpret_cast<WCHAR*>(dwMonsterNamePtr);
						wsMonsterName += L" ";
					}

					LOG_C_D(L"����[%s] ��ɫ=%d", wsMonsterName.c_str(), emColor);
				}*/
				else if (itm.wsIndexName == "Actor")
				{
					LOG_C_D(L"�ɹ���״̬=%d", ReadBYTE(dwIndexObject + 0x80));
				}
				else if (itm.wsIndexName == "WorldItem")
				{
					DWORD dwWorldItemObjectPtr = ReadDWORD(dwIndexObject + 0x14);
					if(dwWorldItemObjectPtr == 0)
						continue;

					LOG_C_D(L"dwWorldItemObjectPtr=%X", dwWorldItemObjectPtr);
					std::vector<ItemAttributeTableContent> VecWorldItemAttributeTable;
					GetItemAttributeTable(dwWorldItemObjectPtr, VecWorldItemAttributeTable);
					for (auto& WorldItem : VecWorldItemAttributeTable)
					{
						DWORD dwWorldItemIndexObject = ReadDWORD(ReadDWORD(dwWorldItemObjectPtr + 0x4) + WorldItem.dwIndex * 4);
						if (WorldItem.wsIndexName == "Base" || WorldItem.wsIndexName == "Render")
						{
							LOG_C_D(L"dwWorldItemIndexObject=%X", dwWorldItemIndexObject);
							DWORD dwNamePtr = ReadDWORD(dwWorldItemIndexObject + 0x8);
							if (ReadDWORD(dwNamePtr + 0x10 + 0x10) == 0)
								continue;

							WCHAR* wszItemNamePtr = reinterpret_cast<WCHAR*>(ReadDWORD(dwNamePtr + 0x10 + 0x14) > 7 ? ReadDWORD(dwNamePtr + 0x10) : (dwNamePtr + 0x10));
							LOG_C_D(L"WorldItemName=%s", wszItemNamePtr);
						}
						else if (WorldItem.wsIndexName == "Mods") // ǿ��װ����ĵ�����Ʒ������Ʒ���� Ĭ������Mods = 2?
						{
							switch (ReadBYTE(dwWorldItemIndexObject + 0x18 + ��Ʒ��ɫƫ��))
							{
							case 0:
								LOG_C_D(L"��Ʒ��ɫ=�װ�");
								break;
							case 1:
								LOG_C_D(L"��Ʒ��ɫ=ħ��");
								break;
							case 2:
								LOG_C_D(L"��Ʒ��ɫ=ϡ��");
								break;
							case 3:
								LOG_C_D(L"��Ʒ��ɫ=����");
								break;
							default:
								LOG_C_D(L"δ֪����Ʒ��ɫ=%d", ReadBYTE(dwWorldItemIndexObject + 0x18 + ��Ʒ��ɫƫ��));
								break;
							}
						}
					}
				}
			}

		}
	});
}

VOID CExpr::ScanBase(CONST std::vector<std::wstring>&)
{
	DWORD dwBase = NULL;
	libTools::CSearchBinary ScanBinary;


	dwBase = ScanBinary.FindBase("B9????????FF??83??????????00", 0x826C4C - 0x826C4C, 1, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define �����ַ 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("E8????????8B??????C6????008B", 0x7BAC08 - 0x7BAC1A, 0x2, 0x0, L"PathOfExile.exe", 0xFF);
	LOG_C_D(L"#define �����ַƫ��1 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("83????000F??????????8B??8B????8D", 0x7BC164 - 0x7BC185, 2, 0x0, L"PathOfExile.exe", 0xFF);
	LOG_C_D(L"#define �����ַƫ��2 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("83????000F??????????8B??8B????8D", 0x7BC164 - 0x7BC193, 2, 0x0, L"PathOfExile.exe", 0xFF);
	LOG_C_D(L"#define �����ַƫ��3 0x%X", 0xFF - dwBase + 1);

	dwBase = ScanBinary.FindBase("85C0????8B??????00008B??????0000F3", 0x7DA8A1 - 0x7DA8CB, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define �����ַƫ��4 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("85C0????8B??????00008B??????0000F3", 0x7DA8A1 - 0x7DA8D1, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define �����ַƫ��5 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("EB0233??83????E8????????8B", 0x677800 - 0x6777D7, 2, 0x0, L"PathOfExile.exe", 0xFF);
	LOG_C_D(L"#define �����ַƫ��6 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("EB0233??83????E8????????8B", 0x677800 - 0x6777E2, 2, 0x0, L"PathOfExile.exe", 0xFF);
	LOG_C_D(L"#define �����ַƫ��7 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("E8????????6A006A006A0033", 0x5DC244 - 0x5DC258, 1, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define �������ֻ�ַ 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("83??14E8????????39??24??74??8B????E8", 0x66C1E8 - 0x66C1FE, 3, 0x0, L"PathOfExile.exe", 0xFF);
	LOG_C_D(L"#define ����ȼ�ƫ�� 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("80????0075??80??????00000075??6A", 0x7E798E - 0x7E797F, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define ��Ʒ����ƫ��1 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("80????0075??80??????00000075??6A", 0x7E798E - 0x7E79A3, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define ��Ʒ����ƫ��2 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("C70000000000C7400400000000C208008B", 0x09447BA - 0x944793, 2, 0x0, L"PathOfExile.exe", 0xFF);
	LOG_C_D(L"#define ��Ʒ����ƫ��3 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("C7????000000008B????3B??7C", 0x865953 - 0x86594A, 2, 0x0, L"PathOfExile.exe", 0xFF);
	LOG_C_D(L"#define ��Ʒ����ƫ��4 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("83??????????000F??????????8B??????????E8????????8B", 0x67BAB8 - 0x67BAD6, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define ��Ʒ��ƫ�� 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("80??????????0075078B??E8????????80????000F", 0x8A1162 - 0x8A1172, 2, 0x0, L"PathOfExile.exe", 0xFF);
	LOG_C_D(L"#define ��Ʒ����ƫ�� 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("80??????????0075078B??E8????????80????000F", 0x8A1162 - 0x8A1156, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define ��Ʒ����ȼ�ƫ�� 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("81????????????7507C6", 0x83C2C5 - 0x83C2DF, 2, 0x0, L"PathOfExile.exe", 0xFF);
	LOG_C_D(L"#define ��Ʒ��ɫƫ�� 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("80??????0000020F??????????8B", 0xA36181 - 0xA36169, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define �ֿ�ƫ��1 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("8B??????00003B??0F??????????8B??????????83", 0x77FA20 - 0x77FA20, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define �ֿ�ƫ��2 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("F30F5805????????8B??F3??????????8B????FF??D8", 0x6E0ABD - 0x6E0AA9, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define �ֿ�ҳƫ��1 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("3B??????????0F84????????83??FF", 0xBC18BD - 0xBC18D8, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define �ֿ�ҳƫ��2 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("8B3C078B87????????3B87????????0F84????????8B308D4D??8B86????????8945??8D45", 0x627D26 - 0x627D29, 2, 2, L"PathOfExile.exe");
	LOG_C_D(L"#define �ֿ�ҳ����ƫ��1 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("8B3C078B87????????3B87????????0F84????????8B308D4D??8B86????????8945??8D45", 0x627D26 - 0x627D40, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define �ֿ�ҳ����ƫ��2 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("E8????????8B??????000083??FF7408", 0xBC1C42 - 0xBC1C47, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define ��ǰ�ֿ�ҳƫ�� 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("E8????????83????8D??????????3B??????6A", 0x658849 - 0x65884E, 2, 0x0, L"PathOfExile.exe", 0xFF);
	LOG_C_D(L"#define ��������ƫ�� 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("E8????????83C4??83????FCE9????????8B????33", 0x92717B - 0x9271A0, 2, 0x0, L"PathOfExile.exe", 0xFF);
	LOG_C_D(L"#define ��Χ�������ƫ��1 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("8D????C7????????????89????89????????8D", 0x68A9D1 - 0x68A9E3, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define ��Χ�������ƫ��2 0x%X", static_cast<DWORD>(abs(static_cast<int>(dwBase))));

	dwBase = ScanBinary.FindBase("FF????8B??8D????????8D????????8D", 0x68ACD1 - 0x68ACB5, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define ��Χ�������ƫ��3 0x%X", dwBase);

	dwBase = ScanBinary.FindBase("FF????8B??8D????????8D????????8D", 0x68ACD1 - 0x68ACBB, 2, 0x0, L"PathOfExile.exe");
	LOG_C_D(L"#define ��Χ�������ƫ��4 0x%X", dwBase);
}
