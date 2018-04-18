#ifndef __LFZL_CHEATDLL_CORE_FEATURE_CONFIG_FILECONFIG_H__
#define __LFZL_CHEATDLL_CORE_FEATURE_CONFIG_FILECONFIG_H__

#include <GameStruct.h>
#include <string>
#include <vector>

class CFileConfig
{
public:
	struct FarmMapContent
	{
		std::vector<std::wstring>	VecFarmMap;
		BOOL						IsUseOpportunityStone; // ʹ�û���ʯ		��ͼ->�����ɫͼ
		BOOL						IsUseDianJinStone;	   // ʹ�õ��ʯ     ��ͼ->��ͼ
		BOOL						IsUseTransformationStone; // ʹ���ɱ�ʯ, ��ͼ->��ͼ
	};


	struct FarmContent
	{
		FarmMapContent				FarmMapInfo;
	};


	struct LoginContent
	{
		std::wstring				wsPlayerName;
	};


	struct AccountConfig
	{
		LoginContent				Login;
		FarmContent					FarmInfo;
		
	};
public:
	CFileConfig() = default;
	~CFileConfig() = default;

	static CFileConfig& GetInstance();

	BOOL Initialize();

	CONST AccountConfig& GetAccountConfig() CONST;
private:
	BOOL ReadFarmConfig();

	BOOL ReadLoginConfig();

	BOOL ReadTextConfig(_In_ LPCWSTR pwszConfigName, _In_ LPCWSTR pwszKeyName, _Out_ std::wstring& wsText) CONST;

	BOOL ReadBoolConfig(_In_ LPCWSTR pwszConfigName, _In_ LPCWSTR pwszKeyName, _Out_ BOOL& bValue) CONST;
private:
	AccountConfig _ConfigContent;
	std::wstring  _wsConfigPath;
};


#endif // !__LFZL_CHEATDLL_CORE_FEATURE_CONFIG_FILECONFIG_H__
