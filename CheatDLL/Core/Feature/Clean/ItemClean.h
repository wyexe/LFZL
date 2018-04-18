#ifndef __LFZL_CHEATDLL_CORE_FEATURE_CLEAN_ITEMCLEAN_H__
#define __LFZL_CHEATDLL_CORE_FEATURE_CLEAN_ITEMCLEAN_H__

#include <GameStruct.h>
#include <Core/Object/Item.h>
#include <vector>

#define WAREHOUSE_MAX_ITEMPAGE_COUNT 3

class CItemClean
{
public:
	// ռһ�����ӵ�ͨ�� ���� ��ͼ ���˿�֮���
	struct ItemCleanContent
	{
		std::wstring		wsItemName;
		em_Object_Type		emObjectType;
		Point				Loc;
		DWORD				dwCount;
		DWORD				dwMaxCount;

		ItemCleanContent(_In_ CONST std::wstring& ItemName, _In_ em_Object_Type emType, _In_ CONST Point& Pos, _In_ DWORD Count, _In_ DWORD MaxCount)
		{
			wsItemName = ItemName;
			emObjectType = emType;
			Loc = Pos;
			dwCount = Count;
			dwMaxCount = MaxCount;
		}
	};

	// �����ֿ�ҳ(��Ʒ)
	struct WarehousePageItemContent
	{
		std::vector<ItemCleanContent>	Vec;
		DWORD							dwItemCount;
		DWORD							dwItemMaxCount;
	};

	// װ��
	struct EquiCleanContent
	{
		std::wstring		wsItemName;
		em_Object_Type		emObjectType;
		CItem::ItemPoint	Loc;
		em_Equi_Color		emColor;
		DWORD				dwLevel;
		BOOL				IsBindAccount;
		BOOL				IsNotAppraisal;

		EquiCleanContent(_In_ CONST std::wstring& wsItemName_, _In_ em_Object_Type emObjectType_, _In_ CONST CItem::ItemPoint& Loc_, _In_ em_Equi_Color emColor_, _In_ DWORD dwLevel_, _In_ BOOL IsBindAccount_, _In_ BOOL IsNotAppraisal_)
		{
			wsItemName = wsItemName_;
			emObjectType = emObjectType_;
			Loc = Loc_;
			emColor = emColor_;
			dwLevel = dwLevel_;
			IsBindAccount = IsBindAccount_;
			IsNotAppraisal = IsNotAppraisal_;
		}
	};

	// �����ֿ�ҳ��װ��
	struct WarehousePageEquiContent
	{
		std::vector<EquiCleanContent>	Vec;
		DWORD							dwItemCount;
		DWORD							dwItemMaxCount;
	};


	// �����ֿ�
	struct WarehouseContent
	{
		std::vector<WarehousePageItemContent>   VecItem;
		std::vector<WarehousePageEquiContent>	VecEqui;

		void Release()
		{
			VecItem.clear();
			VecEqui.clear();
		}

	};
public:
	CItemClean();
	~CItemClean();

	static CItemClean& GetInstance();

	// ��ʼ����ʱ���ȡ�ֿ������
	BOOL InitializeWarehouse();

	// ����������Ʒ�����ֿ�
	BOOL BagItemCleanToWarehouse();

private:
	// ������Ʒ
	VOID DropBagItem() CONST;

	// �ƶ����ֿ�
	BOOL MoveToWarehouse() CONST;

	// �򿪲ֿ�
	BOOL OpenWarehouse() CONST;

	// �����ֿ��������Ʒ
	BOOL TraverseWarehouse();

	//
	VOID FillItemToWarehouseContent(_In_ CONST std::vector<CItem>& Vec, _Out_ WarehousePageItemContent& PageItem) CONST;

	//
	VOID FillEquiToWarehouseContent(_In_ CONST std::vector<CItem>& Vec, _Out_ WarehousePageEquiContent& PageEqui) CONST;

	//
	BOOL ClickWarehousePage(_In_ DWORD Index) CONST;

	//
	BOOL SaveItemToWarehouse(_In_ CONST CItem& itm);

	//
	BOOL SaveEquiToWarehouse(_In_ CONST CItem& equi);

	//
	VOID ChoiceDestory() CONST;

	//
	BOOL IsShowDestoryConform() CONST;

	// ���������ͳ�Ҫˢ��ͼ
	BOOL TakeTheMap_For_Warehouse() CONST;

	// �ڲֿ����ó���Ʒ
	BOOL TakeItem_For_Warehouse(_In_ CONST std::wstring& wsItemName) CONST;

	// �ڲֿ�������Ʒ
	BOOL RightClickItem_In_Warehouse(_In_ CONST std::wstring& wsItemName) CONST;

	// �ڲֿ�������Ʒ
	BOOL FindItem_In_Warehouse_By_ItemName(_In_ CONST std::wstring& wsItemName, _Out_ CItem& Item) CONST;
private:
	WarehouseContent _WarehouseContent;
	UINT			 _uMaxPageCount = 0;
};



#endif // !__LFZL_CHEATDLL_CORE_FEATURE_CLEAN_ITEMCLEAN_H__
