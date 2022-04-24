#pragma once
#include <Windows.h>
#include <string>
#include <set>

using namespace std;

extern const WCHAR* DefaultFontFamily;

class Font {
protected:
	wstring font_family;
	int size;
	HFONT font=0;
public:
	COLORREF textColor = RGB(0, 0, 0);

	Font(int size=12,wstring font_family= DefaultFontFamily);
	void SetFontSize(int size);
	void SetFontFamily(wstring FontFamily);
	void CreateLogFont(HDC hdc);
	void DeleteFont();
	void SetTextColor(COLORREF color);
	int GetFontHeight(HDC hdc);
	~Font();
};

void AddFontFamily(const wstring& FontFamily);


extern set<wstring> FontFiles;//загруженные пользователем шрифты в приложение

void FreeFontResourses();