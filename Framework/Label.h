#pragma once
#include "Frame.h"
#include "TextDrawing.h"

namespace Label {
	extern wstring ClassName;

	void InitWindowClass();

	//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
}

//enum class TextAlign {Center,Left,Right};

class LabelClass :public Frame {
protected:
	TextClass* caption=new TextClass(L"");
	friend void LabelClass_OnPaint(Frame* frame, WPARAM wParam, LPARAM lParam);
public:
	bool isTransparentBackground = false;

	LabelClass(wstring Caption, Position pos, int x, int y, int w, int h, Frame* parent);
	~LabelClass();
	void SetCaption(wstring Caption);
	wstring GetCaption();
	void SetTextAlign(TextAlign TA);
	void SetTextColor(COLORREF color);
	void SetTransparetnBk(bool transparent);
	void SetSingleLine(bool isSingle);
	void CorrectTransparentBkColor();//функция прозрачного блиттинга некорректно работает
};

