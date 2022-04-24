#pragma once
#include "Frame.h"
#include <algorithm>

namespace Edit {
	extern wstring ClassName;
	void InitWindowClass();
	//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
}

class EditClass:public Frame {
protected:
	wstring text;
	int startChar = 0;//смещение в тексте на линии
	int mainLinePos = 0;//позиция первого символа в главной строке(которая первая отображается)
	int linePosY = 0;//начальное смещение в строке, где каретка
	int caretH = 0;
	int caretDy = 0;
	int currentLine = 0;
	bool isDel = 0;//костыль
	bool isEnable = 1;
	bool isMemo=0;//является ли многострочным
public:
	enum class CaretMove {Left,Right,Top,Btm};
	int caretDx = 0;
	void InsertChar(wchar_t ch);
	void DeleteChar();
	void MoveCaret(CaretMove);
	void DeleteCaret();
	void FillEditString(HDC hdc, RECT& r);//заполняет одну строчку
	void DrawEditCaret(HDC hdc);


	EditClass(wstring Text, Position pos, int x, int y, int w, int h, Frame* parent);
	void SetTextColor(COLORREF color);
	void SetEnable(bool isEnable);
	void GetText(wstring& text);
	void SetText(const wstring text);
	void SetMemo(bool isMemo);
};