#ifndef __LFZL_CPPINTERFACE_CORE_FEATURE_ATTRIBUTE_WAREHOUSE_WAREHOUSEATTRIBUTE_H__
#define __LFZL_CPPINTERFACE_CORE_FEATURE_ATTRIBUTE_WAREHOUSE_WAREHOUSEATTRIBUTE_H__

#include <GameStruct.h>
#include <vector>

class CItem;
class CWarehouseAttribute
{
public:
	CWarehouseAttribute() = default;
	~CWarehouseAttribute() = default;

	static BOOL IsOpenedWarehouse();

	// �ܹ���ҳ�Ĳֿ�
	static DWORD GetWarehousePageCount();

	// ��ǰ�򿪵��ǵڼ�ҳ�ֿ�
	static DWORD GetCurrentPageIndex();

	// ��ǰ����ҳ�Ĳֿ�ҳ����
	static DWORD GetCurrentPageObject();

	// ��ȡ��ǰ�ֿ�ҳ����Ʒ
	static UINT GetCurrentPageItem(_Out_ std::vector<CItem>& Vec);
private:
	static DWORD GetWarehouseNodeBase();

	static DWORD GetWarehouseObject();
};


#endif // !__LFZL_CPPINTERFACE_CORE_FEATURE_ATTRIBUTE_WAREHOUSE_WAREHOUSEATTRIBUTE_H__
