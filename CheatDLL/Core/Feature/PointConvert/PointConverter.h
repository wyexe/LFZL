#ifndef __LFZL_CHEATDLL_CORE_FEATURE_POINTCONVERT_POINTCONVERTER_H__
#define __LFZL_CHEATDLL_CORE_FEATURE_POINTCONVERT_POINTCONVERTER_H__

#include <GameStruct.h>


class CPointConverter
{
public:
	CPointConverter() = default;
	~CPointConverter() = default;

	// ��ȡ��ǰ������Ļ����
	static Point GetCursorClientPos();

	// ��ȡ��ǰ������Ϸ����
	static Point GetCursorGamePos();

	// ����Ļ����ת������Ϸ����
	static Point ConvertClientPosToGamePos(_In_ CONST Point& ClientPos);

	// ����Ϸ����ת������Ļ����
	static BOOL ConvertGamePosToClientPos(_In_ CONST Point& ClientPos, _Out_ Point& GamePos);
private:

};



#endif // !__LFZL_CHEATDLL_CORE_FEATURE_POINTCONVERT_POINTCONVERTER_H__
