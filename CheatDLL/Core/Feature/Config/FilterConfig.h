#ifndef __LFZL_CHEATDLL_CORE_FEATURE_CONFIG_FILTERCONFIG_H__
#define __LFZL_CHEATDLL_CORE_FEATURE_CONFIG_FILTERCONFIG_H__

#include <GameStruct.h>

class CWorldItem;
class CMonster;
class CItem;
class CFilterConfig
{
public:
	CFilterConfig() = default;
	~CFilterConfig() = default;

	static CFilterConfig& GetInstance();
public:
	BOOL Initialize();
public:
	// �Ƿ�ʰȡ������Ʒ
	BOOL IsTakeWorldItem(_In_ CONST CWorldItem& WorldItem) CONST;

	// �Ƿ�������Ʒ(����|�ֿ�)
	BOOL IsKeepItem(_In_ CONST CItem& Item) CONST;

	// �Ƿ񹥻��ù���
	BOOL IsAttackMonster(_In_ CONST CMonster& Monster) CONST;
private:

};



#endif // !__LFZL_CHEATDLL_CORE_FEATURE_CONFIG_FILTERCONFIG_H__
