#pragma once
#include <Windows.h>


class BackBuffer {
protected:
	HDC hBuffer;
	HBITMAP btmBuffer;
public:
	BackBuffer();
	HDC CreateBuffer(HDC hdc,const RECT& r);
	void Blt(HDC hdc,const RECT& r,bool isTransparent=false,COLORREF transparentColor=RGB(0,0,0));
	void DeleteBuffer();
	~BackBuffer();
};