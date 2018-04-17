#include "PersonAction.h"
#include "GameMouse.h"
#include <ProcessLib/KeyboardMsg/KeyboardMsg.h>
#include <TimeLib/TimeRand.h>
#include <LogLib/Log.h>

#define _SELF L"PersonAction.cpp"
CPersonAction& CPersonAction::GetInstance()
{
	static CPersonAction Instance;
	return Instance;
}

VOID CPersonAction::UseSkill(_In_ em_Skill_Id emSkillId)
{
	switch (emSkillId)
	{
	case CPersonAction::em_Skill_Id::Q:
		libTools::CKeyboardMsg::SendKey('Q');
		break;
	case CPersonAction::em_Skill_Id::W:
		libTools::CKeyboardMsg::SendKey('W');
		break;
	case CPersonAction::em_Skill_Id::E:
		libTools::CKeyboardMsg::SendKey('E');
		break;
	case CPersonAction::em_Skill_Id::R:
		libTools::CKeyboardMsg::SendKey('R');
		break;
	case CPersonAction::em_Skill_Id::T:
		libTools::CKeyboardMsg::SendKey('T');
		break;
	default:
		LOG_MSG_CF(L"��Ч��emSkillId in CPersonAction::UseSkill");
		break;
	}
}

VOID CPersonAction::UseMouseSkill(_In_ em_Skill_Id emSkillId)
{
	switch (emSkillId)
	{
	case CPersonAction::em_Skill_Id::LeftClick:
		MouseClick();
		break;
	case CPersonAction::em_Skill_Id::RightClick:
		mouse_event(MOUSEEVENTF_RIGHTDOWN, NULL, NULL, NULL, NULL);
		::Sleep(libTools::CTimeRand::GetRand(100, 300));
		mouse_event(MOUSEEVENTF_RIGHTUP, NULL, NULL, NULL, NULL);
		break;
	case CPersonAction::em_Skill_Id::MiddleClick:
		mouse_event(MOUSEEVENTF_MIDDLEDOWN, NULL, NULL, NULL, NULL);
		::Sleep(libTools::CTimeRand::GetRand(100, 300));
		mouse_event(MOUSEEVENTF_MIDDLEUP, NULL, NULL, NULL, NULL);
		break;
	default:
		LOG_MSG_CF(L"��Ч��emSkillId in CPersonAction::UseSkill");
		break;
	}
}

VOID CPersonAction::MouseClick()
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
	::Sleep(libTools::CTimeRand::GetRand(100, 300));
	mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
}

VOID CPersonAction::MouseMove(_In_ CONST Point& Pos)
{
	CGameMouse::GetInstance().MoveTo(Pos);
}