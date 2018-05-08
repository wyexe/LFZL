#include "ItemClean.h"
#include <Core/Feature/Attribute/Warehouse/WarehouseAttribute.h>
#include <Core/Feature/Attribute/Bag/BagAttribute.h>
#include <Core/Feature/EchoAction/PersonAction.h>
#include <Core/Feature/Config/FileConfig.h>
#include <Core/Feature/Searcher/ObjectFilter.h>
#include <Core/Object/Npc.h>
#include <Core/Object/Person.h>
#include <TimeLib/TimeTick.h>
#include <LogLib/Log.h>

#define _SELF L"ItemClean.cpp"
CItemClean::CItemClean()
{
}

CItemClean::~CItemClean()
{
}

CItemClean& CItemClean::GetInstance()
{
	static CItemClean Instance;
	return Instance;
}

BOOL CItemClean::InitializeWarehouse()
{
	if (!OpenWarehouse())
	{
		LOG_C_E(L"OpenWarehouse = FALSE");
		return FALSE;
	}

	return TraverseWarehouse();
}

BOOL CItemClean::BagItemCleanToWarehouse()
{
	DropBagItem();
	if (!OpenWarehouse())
	{
		LOG_C_E(L"OpenWarehouse = FALSE");
		return FALSE;
	}


	std::vector<CItem> VecBagItem;
	CBagAttribute::GetVecBagItem(VecBagItem);

	for (UINT i = 0;i < VecBagItem.size() && GameRun; ++i)
	{
		auto& itm = VecBagItem.at(i);
		auto emEchoType = itm.GetEchoItemType();
		switch (emEchoType)
		{
		case CItem::em_EchoItem_Type::Item:
			SaveItemToWarehouse(itm);
			break;
		case CItem::em_EchoItem_Type::Equi:
			SaveEquiToWarehouse(itm);
			break;
		case CItem::em_EchoItem_Type::Keep:
			break;
		default:
			break;
		}
	}
	return TRUE;
}

VOID CItemClean::DropBagItem() CONST
{
	if (!OpenBag())
	{
		LOG_C_E(L"OpenBag = FALSE");
		return;
	}

	std::vector<CItem> VecBagItem;
	CBagAttribute::GetVecBagItem(VecBagItem);
	for (CONST auto& itm : VecBagItem)
	{
		CONST static std::vector<Point> VecDropPoint = 
		{
			Point(777,254), Point(749,389), Point(753,651),
			Point(550,192), Point(577,444), Point(575,683),
		};


		for (auto& DropPoint : VecDropPoint)
		{
			itm.Click(CItem::em_ItemLocation_Type::Bag);
			::Sleep(1000);


			CPersonAction::MouseMoveAndClick(DropPoint);

			// �ж��Ƿ���ֶ���ȷ�ϴ���
			if (IsShowDestoryConform())
			{
				ChoiceDestory();
			}
		}
	}
}

BOOL CItemClean::MoveToWarehouse() CONST
{
	// �ж��Լ�����
	if (CPerson::GetInstance().GetMapName() != L"������")
	{
		LOG_C_D(L"�������[%s], ��Ҫ���͹���!", CPerson::GetInstance().GetMapName().c_str());
		// #
		// ��������, ��Ҫ���ô��͹����� 
	}

	CNpc Npc;
	if (!CObjectFilter::FindNpc_By_Name(L"�ֿ�", Npc))
	{
		LOG_C_E(L"��Χû�ҵ��ֿ�!");
		return FALSE;
	}
	else if (!Npc.MoveToPos())
	{
		LOG_C_E(L"�ߵ�NPC[%s] ʧ��!", Npc.GetName().c_str());
		return FALSE;
	}


	return TRUE;
}

BOOL CItemClean::OpenWarehouse() CONST
{
	if (!MoveToWarehouse())
	{
		LOG_C_E(L"MoveToWarehouse = FALSE!");
		return FALSE;
	}

	libTools::CTimeTick TimeTick;
	while (GameRun && TimeTick.GetSpentTime(libTools::CTimeTick::em_TimeTick::em_TimeTick_Second) < 60)
	{
		// �����ֿ�Npc
		CNpc Npc;
		if (!CObjectFilter::FindNpc_By_Name(L"�ֿ�", Npc))
		{
			LOG_C_E(L"��Χû�ҵ��ֿ�!");
			break;
		}

		// ����ƶ����ֿ� ���
		Npc.Click();
		::Sleep(1000);
		if (CWarehouseAttribute::IsOpenedWarehouse())
		{
			LOG_C_D(L"�򿪲ֿ����!");
			return TRUE;
		}
	}


	LOG_C_E(L"�򿪲ֿⳬʱ!");
	return FALSE;
}

BOOL CItemClean::TraverseWarehouse()
{
	_WarehouseContent.Release();


	for (DWORD i = 0;i < WAREHOUSE_MAX_ITEMPAGE_COUNT; ++i)
	{
		// ��һҳ��ͨ��
		if (!ClickWarehousePage(i))
		{
			LOG_C_D(L"ClickWarehousePage[%d] Falid!", i);
			return FALSE;
		}


		std::vector<CItem> VecItem;
		CWarehouseAttribute::GetCurrentPageItem(VecItem);


		WarehousePageItemContent Page;
		FillItemToWarehouseContent(VecItem, Page);
		Page.dwItemMaxCount = CWarehouseAttribute::GetCurrentWarehousePageMaxItemCount();
		_WarehouseContent.VecItem.push_back(Page);	
	}

	_uMaxPageCount = CWarehouseAttribute::GetWarehousePageCount();
	for (DWORD i = WAREHOUSE_MAX_ITEMPAGE_COUNT; i < _uMaxPageCount; ++i)
	{
		// ����-����ҳ��װ��
		if (!ClickWarehousePage(i))
		{
			LOG_C_D(L"ClickWarehousePage[%d] Falid!", i);
			return FALSE;
		}

		std::vector<CItem> VecItem;
		CWarehouseAttribute::GetCurrentPageItem(VecItem);

		WarehousePageEquiContent VecEqui;
		FillEquiToWarehouseContent(VecItem, VecEqui);
		VecEqui.dwItemMaxCount = CWarehouseAttribute::GetCurrentWarehousePageMaxItemCount();
		_WarehouseContent.VecEqui.push_back(VecEqui);
	}


	return TRUE;
}

VOID CItemClean::FillItemToWarehouseContent(_In_ CONST std::vector<CItem>& Vec, _Out_ WarehousePageItemContent& PageItem) CONST
{
	for (CONST auto& itm : Vec)
	{
		auto Loc = itm.GetItemLocation();
		PageItem.Vec.emplace_back(itm.GetName(), itm.GetType(), Point(Loc.dwLeftTopIndex, Loc.dwRightTopIndex), itm.GetCount(), itm.GetMaxCount());
	}

	PageItem.dwItemCount = Vec.size();
}

VOID CItemClean::FillEquiToWarehouseContent(_In_ CONST std::vector<CItem>& Vec, _Out_ WarehousePageEquiContent& PageEqui) CONST
{
	DWORD dwItemCount = 0;
	for (CONST auto& itm : Vec)
	{
		auto Loc = itm.GetItemLocation();
		PageEqui.Vec.emplace_back(itm.GetName(), itm.GetType(), Loc, itm.GetEquiColor(), itm.GetEquiLevel(), itm.IsBindAccount(), itm.IsNotAppraisal());

		dwItemCount += (Loc.dwLeftBottomIndex - Loc.dwLeftTopIndex) * (Loc.dwRightBottomIndex - Loc.dwRightTopIndex);
	}
}

BOOL CItemClean::ClickWarehousePage(_In_ DWORD Index) CONST
{
	CONST static Point PageOffset(133, 133);
	CONST static DWORD dwPageIndexOffset = 44;


	libTools::CTimeTick TimeTick;
	while (GameRun && TimeTick.GetSpentTime(libTools::CTimeTick::em_TimeTick::em_TimeTick_Second) < 60)
	{
		if (CWarehouseAttribute::GetCurrentPageIndex() == Index)
		{
			LOG_C_D(L"�ɹ��л����ֿ�ҳ��[%d]", Index);
			return TRUE;
		}
		else if (!CWarehouseAttribute::IsOpenedWarehouse())
		{
			LOG_C_E(L"����ֿ�ҳ���ʱ�򡭡��ֿⲻ֪��Ϊë����");
			return FALSE;
		}

		CPersonAction::MouseMoveAndClick(Point(PageOffset.X + Index * dwPageIndexOffset, PageOffset.Y));
	}

	LOG_C_E(L"ClickWarehousePage Timeout");
	return FALSE;
}

BOOL CItemClean::SaveItemToWarehouse(_In_ CONST CItem& itm)
{
	for (DWORD i = 0;i < WAREHOUSE_MAX_ITEMPAGE_COUNT; ++i)
	{
		// �㿪��Nҳ
		auto Loc = itm.GetItemLocation();
		if (!ClickWarehousePage(i))
		{
			LOG_C_E(L"ClickWarehousePage = FALSE");
			return FALSE;
		}

		// �ƶ���굽����Ʒ��
		itm.CtrlClick(CItem::em_ItemLocation_Type::Bag);
		::Sleep(1000);

		// �жϸ���Ʒ���ڲ���
		if (!CBagAttribute::ExistItem_By_Location(Loc))
		{
			return TRUE;
		}
	}
	
	LOG_MSG_CF(L"��Ʒ�ֿ�ҳ����!");
	StopGame;
	return FALSE;
}

BOOL CItemClean::SaveEquiToWarehouse(_In_ CONST CItem& equi)
{
	for (UINT i = WAREHOUSE_MAX_ITEMPAGE_COUNT; i < _uMaxPageCount; ++i)
	{
		// �㿪��Nҳ
		auto Loc = equi.GetItemLocation();
		if (!ClickWarehousePage(i))
		{
			LOG_C_E(L"ClickWarehousePage = FALSE");
			return FALSE;
		}

		// �ƶ���굽����Ʒ��
		equi.CtrlClick(CItem::em_ItemLocation_Type::Bag);
		::Sleep(1000);

		// �жϸ���Ʒ���ڲ���
		if (!CBagAttribute::ExistItem_By_Location(Loc))
		{
			return TRUE;
		}
	}

	LOG_MSG_CF(L"װ���ֿ�ҳ����!");
	StopGame;
	return FALSE;
}

VOID CItemClean::ChoiceDestory() CONST
{

	do 
	{
		LOG_C_D(L"����ѡ����...");
		CPersonAction::MouseMoveAndClick(Point(545, 424));
		::Sleep(1000);
	} while (IsShowDestoryConform());
}

BOOL CItemClean::IsShowDestoryConform() CONST
{
	return CUiAttribute::IsShow(CUiAttribute::em_Ui_Type::DestoryItemConfirm);
}

BOOL CItemClean::TakeTheMap_For_Warehouse() CONST
{
	CONST auto& AccConfig = CFileConfig::GetInstance().GetAccountConfig();
	CONST auto& VecFarmMap = AccConfig.FarmInfo.FarmMapInfo.VecFarmMap;
	for (int i  = 0;i < static_cast<int>(VecFarmMap.size()); ++i)
	{
		CONST auto& MapName = VecFarmMap.at(i);
		if (!TakeItem_For_Warehouse(MapName))
		{
			continue;
		}


		CItem ItemMap;
		if (!CBagAttribute::FindItem_By_Name(MapName, ItemMap))
		{
			LOG_C_E(L"FindItem_By_Name(%s) = FALSE, retry!", MapName.c_str());
			i -= 1;
			continue;
		}


		if (ItemMap.GetEquiColor() == em_Equi_Color::White)
		{
			std::wstring wsStoneName;
			if (AccConfig.FarmInfo.FarmMapInfo.IsUseDianJinStone)
			{
				wsStoneName = L"���ʯ";
			}
			else if (AccConfig.FarmInfo.FarmMapInfo.IsUseOpportunityStone)
			{
				wsStoneName = L"����ʯ";
			}
			else if (AccConfig.FarmInfo.FarmMapInfo.IsUseTransformationStone)
			{
				wsStoneName = L"�ɱ�ʯ";
			}
			else
			{
				// ʹ�ð�ͼ
				continue;
			}


			if (!RightClickItem_In_Warehouse(wsStoneName))
			{
				i -= 1;
				continue;
			}

			ItemMap.Click(CItem::em_ItemLocation_Type::Bag); // ����ͼ��ɫ����?
			if (CBagAttribute::GetItemColor_By_Name(MapName) <= 1) // ���ڰ�ɫ
			{
				LOG_C_E(L"��ͼ��ɫʧ�ܡ��� retry !");
				i -= 1;
				continue;
			}
		}

		LOG_C_D(L"׼��ˢ���[%s]", MapName.c_str());
		return TRUE;
	}

	LOG_MSG_CF(L"Ҫˢ�ĵ�ͼľ����!");
	return FALSE;
}

BOOL CItemClean::TakeItem_For_Warehouse(_In_ CONST std::wstring& wsItemName) CONST
{
	CItem Item;
	if (!FindItem_In_Warehouse_By_ItemName(wsItemName, Item))
	{
		return FALSE;
	}


	Item.CtrlClick(CItem::em_ItemLocation_Type::Warehouse);
	return TRUE;
}

BOOL CItemClean::RightClickItem_In_Warehouse(_In_ CONST std::wstring& wsItemName) CONST
{
	CItem Item;
	if (!FindItem_In_Warehouse_By_ItemName(wsItemName, Item))
	{
		return FALSE;
	}


	// �������Ƿ��� �����Ʒ״̬�ˡ���
	libTools::CTimeTick TimeTick;
	while (TimeTick.GetSpentTime(libTools::CTimeTick::em_TimeTick::em_TimeTick_Second) < 30)
	{
		if (CUiAttribute::IsShow(CUiAttribute::em_Ui_Type::MouseUseItemFlag))
		{
			return TRUE;
		}
		Item.RightClick(CItem::em_ItemLocation_Type::Warehouse);
	}


	LOG_C_E(L"Right Warehouse Item Timeout!");
	return FALSE;
}

BOOL CItemClean::FindItem_In_Warehouse_By_ItemName(_In_ CONST std::wstring& wsItemName, _Out_ CItem& Item) CONST
{
	for (UINT i = 0; i < _uMaxPageCount; ++i)
	{
		// �㿪��Nҳ
		if (!ClickWarehousePage(i))
		{
			LOG_C_E(L"ClickWarehousePage = FALSE");
			return FALSE;
		}


		std::vector<CItem> VecItem;
		CWarehouseAttribute::GetCurrentPageItem(VecItem);
		for (CONST auto& itm : VecItem)
		{
			if (itm.GetName() == wsItemName)
			{
				Item = itm;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CItemClean::OpenBag() CONST
{
	libTools::CTimeTick TimeTick;
	while (TimeTick.GetSpentTime( libTools::CTimeTick::em_TimeTick::em_TimeTick_Second) < 60)
	{
		if (CUiAttribute::IsShow(CUiAttribute::em_Ui_Type::BagDialog))
		{
			return TRUE;
		}


		CPersonAction::OpenBag();
		::Sleep(1000);
	}

	LOG_C_E(L"�򿪱�����ʱ!");
	return FALSE;
}
