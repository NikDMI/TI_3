#pragma once
#include "Frame.h"

class Painter {
private:
	Border border_top{ 0 };
	Border border_bottom{ 0 };
	Border border_left{ 0 };
	Border border_right{ 0 };
	COLORREF bk_color;
	HBITMAP background_picture = 0;
	bool isTrasparentPic = false;
	COLORREF transparent_color;
	DrawFunction ownerDraw = NULL;//функция принудительного рисования
	friend class Frame;
public:
	void SetBorder(Border border, BorderNum bn);
	void DrawBorder(HDC hdc, RECT& r);
	void SetBackgroundColor(COLORREF cl);
	void FillBkColor(HDC hdc, RECT& r);
	void PaintWindow(HDC hdc, RECT& rect);//вызывает базовые процедуры для отрисовки окна
	void SetPicture(HBITMAP bmp, bool isTransparent = 0, COLORREF transparent_color = RGB(255, 255, 255));
	void SetOwnerDraw(DrawFunction df);
};

#define count_painter_types 4
enum painterType {Static,Active,Move,Pushed};

class ComplexPainter {
private:
	Painter* painters[count_painter_types];
	bool isAutoClear[count_painter_types];
public:
	~ComplexPainter();
	ComplexPainter();
	void SetPainter(painterType type, Painter* painter, bool isAutoClear);
	void ActivatePainter(painterType type, HDC hdc, RECT& r);
	Painter* GetPainter(painterType type);
};
