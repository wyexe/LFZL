#include "GameMouse.h"
#include <cmath>
#include <LogLib/Log.h>
#include <ProcessLib/KeyboardMsg/KeyboardMsg.h>
#include <TimeLib/TimeRand.h>

#pragma comment(lib,"TimeLib.lib")

#define _SELF L"GameMouse.cpp"
CGameMouse& CGameMouse::GetInstance()
{
	static CGameMouse Instance;
	return Instance;
}


VOID CGameMouse::MoveTo(_In_ CONST Point& Pos)
{
	MoveToPoint(AddGameClientPos(Pos));
}

VOID CGameMouse::MoveToClient(_In_ CONST Point& Pos)
{
	MoveToPoint(Pos);
}

Point CGameMouse::AddGameClientPos(_In_ CONST Point& Pos) CONST
{
	HWND hGameWnd = ::FindWindowW(L"POEWindowClass", L"Path of Exile");
	if (hGameWnd == NULL)
	{
		LOG_MSG_CF(L"Find Game Wnd = NULL");
		return Pos;
	}


	RECT GameClientRect;
	if (!::GetWindowRect(hGameWnd, &GameClientRect))
	{
		LOG_MSG_CF(L"GetWindowRect = NULL");
		return Pos;
	}


	return Point(Pos.X + GameClientRect.left, Pos.Y + GameClientRect.top);
}

VOID CGameMouse::MoveToPoint(_In_ CONST Point& Pos) CONST
{
	POINT CursorPos;
	if (!::GetCursorPos(&CursorPos))
	{
		LOG_MSG_CF(L"GetCursorPos = FALSE!");
		return;
	}

	tagMovePoint A(CursorPos.x, CursorPos.y);
	tagMovePoint B(Pos.X, Pos.Y);

	switch (libTools::CTimeRand::GetRand(1, 5))
	{
	case 2:
		LineMove(A, B);
		break;
	case 4:
		RectangleMove(A, B);
		break;
	default:
		LineMove(A, B);
		break;
	}
}

VOID CGameMouse::CircelMove(_In_ CONST tagMovePoint& A, _In_ CONST tagMovePoint& B) CONST
{
	// �赱ǰ��������=A�� �յ�����=B 
	// Բ��ֱ�� ^ 2 = (A.X - B.X)^2 + (A.Y - B.Y) ^ 2
	float L = sqrt(pow(static_cast<float>(abs(A.X - B.X)), 2) + pow(static_cast<float>(abs(A.Y - B.Y)), 2));
	// �뾶
	float r = L / 2;
	// Բ��(a,b) = [A.X + (B.X - A.X), (A.Y + (B.Y - A.Y))]
	
	tagMovePoint CircleCenter((A.X + B.X) / 2, (A.Y + B.Y) / 2);
	LOG_C_D(L"A(%d,%d) B(%d,%d) r(%d,%d) r[%d] L[%d]", A.X, A.Y, B.X, B.Y, CircleCenter.X, CircleCenter.Y, (int)r,(int)L);
#define PI 3.141592
	for (int Angle = 0; Angle <= 180; ++Angle)
	{
		// ��Բ���ϵ�ÿһ����C(x,y)���� (x - a)^2 + (y - b)^2 = r^2
		// �����Ǻ��� x = a + r * cos(Angle)
		// y = b + r * sin(Angle)
		int X = B.X > A.X ? static_cast<int>(CircleCenter.X + r * cos(PI / 180 * Angle)) : static_cast<int>(CircleCenter.X - r * cos(PI / 180 * Angle));
		int Y = B.Y > A.Y ? static_cast<int>(CircleCenter.Y + r * sin(PI / 180 * Angle)) : static_cast<int>(CircleCenter.Y - r * sin(PI / 180 * Angle));
		::SetCursorPos(X, Y);
		RandSleep();
	}

	::MessageBoxW(NULL, L"End!", L"", NULL);
	::SetCursorPos(B.X, B.Y);
}

VOID CGameMouse::LineMove(_In_ CONST tagMovePoint& A, _In_ CONST tagMovePoint& B) CONST
{
	// ���2���ľ���
	float L = sqrt(pow(static_cast<float>(abs(A.X - B.X)), 2) + pow(static_cast<float>(abs(A.Y - B.Y)), 2));

	// ÿ���ƶ�5 �ľ���
	CONST static float nDis = 5;

	// ����������
	float fAngleX = (B.X - A.X) / L;
	float fAngleY = (B.Y - A.Y) / L;

	
	for (float i = nDis; i < L; i += nDis)
	{
		int X = static_cast<int>(fAngleX * i);
		int Y = static_cast<int>(fAngleY * i);

		::SetCursorPos(A.X + X, A.Y + Y);
		RandSleep();
	}


	::SetCursorPos(B.X, B.Y);
}

VOID CGameMouse::RectangleMove(_In_ CONST tagMovePoint& A, _In_ CONST tagMovePoint& B) CONST
{
	// ÿ���ƶ�5 �ľ���
	CONST static int nDis = 5;


	// ����ˮƽ�����ƶ���B
	int nHorizontalDis = abs(B.X - A.X);
	bool IsMoveToRight = B.X > A.X ? true : false;
	for (int i = nDis; i < nHorizontalDis; i += nDis)
	{
		::SetCursorPos(IsMoveToRight ? A.X + i : A.X - i, A.Y);
		RandSleep();
	}
	::SetCursorPos(B.X, A.Y);


	// ��ֱ����
	int nNverticalDis = abs(B.Y - A.Y);
	bool IsMoveToBottom = B.Y > A.Y ? true : false;
	for (int i = nDis; i < nNverticalDis; i += nDis)
	{
		::SetCursorPos(B.X, IsMoveToBottom ? A.Y + i : A.Y - i);
		RandSleep();
	}

	::SetCursorPos(B.X, B.Y);
}

VOID CGameMouse::RandSleep() CONST
{
	::Sleep(libTools::CTimeRand::GetRand(1, 5));
}
