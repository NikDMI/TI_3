#pragma once
#include "Frame.h"
#include "Painter.h"
//#include <gdiplus.h>


namespace ScrollBar {
	extern wstring ClassName;
	void InitWindowClass();
}

using ScrollEvent = void (*)(Frame* scroll,int thumbPos);
enum ScrollBarType {Vertical,Horizontal};

class ScrollBarClass :public Frame {
protected:
	int min = 0;
	int max = 101;
	int range=max-min;
	int thumbPos = 0;
	int thumbSize = 70;//размер в пикселях
	bool isScrolling = 0;
	bool isMouseOn=0;
	ScrollEvent scEvent=nullptr;
	ScrollBarType sbType=ScrollBarType::Vertical;
	int dT = -1;//движение
	int startPos = 0;

	class BadRangeException {};
	void DrawThumb(HDC hdc, RECT clipRect);

	friend void ScrollBarClass_OnPaint(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void ScrollBarClass_OnMove(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void ScrollBarClass_OnLeave(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void ScrollBarClass_OnLDown(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void ScrollBarClass_OnLUp(Frame* frame, WPARAM wParam, LPARAM lParam);
public:
	ComplexPainter* complexPainter = new ComplexPainter();
	ScrollBarClass(Position pos, int x, int y, int w, int h, Frame* parent);
	~ScrollBarClass();
	void SetScrollInfo(int min, int max,int thumbPos,int thumbSize);
	void SetScrollEvent(ScrollEvent sc);
	void SetScrollType(ScrollBarType type);
};