#pragma once
#include "CWindow.h"
#include "TextDrawing.h"

namespace CaptionClassNS {
	extern wstring ClassName;
	void InitWindowClass();
}

class CaptionClass :public WindowClass {
protected:
	int xPressed, yPressed;//координаты мыши при зажитии
	bool isMoved = false;//передвигается ли окно
	friend void CaptionClass_OnPaint(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void CaptionClass_OnMove(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void CaptionClass_OnLeave(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void CaptionClass_OnLDown(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void CaptionClass_OnLUp(Frame* frame, WPARAM wParam, LPARAM lParam);
public:
	TextClass* caption;
	CaptionClass(Position pos, int x, int y, int w, int h, Frame* parent);
	CaptionClass(int h, Frame* parent);
	~CaptionClass() { delete caption; };
};
