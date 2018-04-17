#include "GameLogin.h"
#include <ProcessLib/Process/Process.h>
#include <TimeLib/TimeTick.h>
#include <LogLib/Log.h>
#include <Core/Feature/GameMemory/GameMemory.h>
#include <Core/Feature/Searcher/ObjectSearcher.h>

#define _SELF L"GameLogin.cpp"
CGameLogin::em_Login_RetCode CGameLogin::Begin()
{
	if (IsTimeOut(20 * 1000, std::bind(&CGameLogin::IsExistGameProc, this)))
	{
		LOG_C_E(L"20s֮�ڶ�û��⵽��Ϸ����");
		return em_Login_RetCode::Faild;
	}


	if (IsTimeOut(20 * 1000, std::bind(&CGameLogin::IsExistGameWindows, this)))
	{
		LOG_C_E(L"20s֮�ڶ�û��⵽��Ϸ����");
		return em_Login_RetCode::Faild;
	}


	if (!CGameMemory::GetInstance().InitializeReadMemory())
	{
		LOG_C_E(L"��ʼ����ȡ�����ڴ�ʧ��!");
		return em_Login_RetCode::Faild;
	}


	if (IsTimeOut(60 * 1000, std::bind(&CGameLogin::IsAlreadyInChoicPlayer, this)))
	{
		LOG_C_E(L"60s֮�ڶ�������ѡ����Ϸ��ɫ!");
		return em_Login_RetCode::Faild;
	}


	return ChoicPlyaerToGame();
}

BOOL CGameLogin::IsExistGameProc() CONST
{
	return libTools::CProcess::IsExistProcName(L"PathOfExile.exe");
}

BOOL CGameLogin::IsTimeOut(_In_ DWORD dwTimeout, _In_ std::function<BOOL(VOID)> Ptr) CONST
{
	BOOL bRetCode = TRUE;
	libTools::CTimeTick TimeTick;
	while (TimeTick.GetSpentTime(libTools::CTimeTick::em_TimeTick::em_TimeTick_Millisecond) < dwTimeout)
	{
		if (Ptr())
		{
			bRetCode = FALSE;
			break;
		}
	}
	return bRetCode;
}

BOOL CGameLogin::IsExistGameWindows() CONST
{
	return ::FindWindowW(L"POEWindowClass", L"Path of Exile") != NULL;
}

BOOL CGameLogin::IsAlreadyInChoicPlayer() CONST
{
	return GetGameScheduleText() == L"SelectCharacterState";
}

BOOL CGameLogin::IsAlreadyInGameState() CONST
{
	return GetGameScheduleText() == L"InGameState";
}

BOOL CGameLogin::IsAreaLoadingState() CONST
{
	return GetGameScheduleText() == L"AreaLoadingState";
}

CGameLogin::em_Login_RetCode CGameLogin::ChoicPlyaerToGame() CONST
{
	libTools::CTimeTick TimeTick;
	while (TimeTick.GetSpentTime(libTools::CTimeTick::em_TimeTick::em_TimeTick_Millisecond) < 60 * 1000)
	{
		::Sleep(1000);
		if (IsAreaLoadingState())
		{
			LOG_C_D(L"��ͼ��...");
			continue;
		}


		if (IsAlreadyInChoicPlayer())
		{
			LOG_C_D(L"ѡ���ɫ��...");
			continue;
		}


		if (IsAlreadyInGameState())
		{
			LOG_C_D(L"������Ϸ���!");
			return em_Login_RetCode::Succ;
		}
	}


	return em_Login_RetCode::Faild;
}

std::wstring CGameLogin::GetGameScheduleText() CONST
{
	DWORD dwGameEnv = CObjectSearcher::GetGameEnv();
	return dwGameEnv == 0 ? L"" : CGameMemory::GetInstance().ReadProcTextWithLength(dwGameEnv + 0x8);
}
