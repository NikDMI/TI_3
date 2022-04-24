#pragma once
#include "Frame.h"
//класс, отвечающий за форматированный вывод текста

enum class TextAlign { Center, Left, Right };
enum class TextVerticalAlign {Top,Bottom,Center};


class TextClass {
private:
	wstring text;
	UINT format=DT_LEFT|DT_SINGLELINE|DT_VCENTER;
	int dx=0, dy=0;//отступы текста
public:
	TextClass(wstring text);
	void SetTextAlign(TextAlign TA);
	void SetTextVerticalAlign(TextVerticalAlign TA);
	void SetSingleLine(bool isSingleLine);
	void SetText(wstring text);
	wstring GetText();
	void SetTextOffset(int dx, int dy);
	void DrawText_(HDC hdc, RECT& r);
};