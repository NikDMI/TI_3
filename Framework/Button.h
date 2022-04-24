#pragma once
#include "Frame.h"
#include "TextDrawing.h"

class Button;


namespace CButton {
	extern wstring ClassName;
	void InitWindowClass();
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
}


class Button :public Frame {
private:
public:
	//wstring Caption;
	TextClass* caption;
	Button(Position pos, int x, int y, int w, int h, Frame* parent);
	~Button() { delete caption; }
	void SetCaption(wstring Caption);
	wstring GetCaption();
	void DrawCaption(HDC hdc, RECT& rect);
};