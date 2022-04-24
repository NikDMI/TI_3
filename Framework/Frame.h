
#pragma once
//������� ����� ���� ���������
#include <Windows.h>
#include <string>
#include <map>
#include <vector>
#include "Font.h"
#include "BackBuffer.h"
#include "EventHandler.h"
#include "WindowHandler.h"
//#include <gdiplus.h>

class Painter;

using namespace std;

class Frame;

enum class Position {absoluteAll,percentAll,absoluteSize,absolutePos,absolutePosSpaceDown,absolutePosH};//��������������

extern int wGlobal, hGlobal;
extern HINSTANCE mainInstance;

void InitClasses();

enum class BorderStyle{solid};
enum class BorderNum {top,btm,left,right};

using DrawFunction = void (*)(HDC hdc, RECT r);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//������� ������� ���������

struct Border {//�������� ������� ���� ��� CSS box-sizing: border-box;
	COLORREF color;
	int width;
	BorderStyle style;
};



class Frame {
protected:
	HINSTANCE hInstance;//���������� ����������, � ������� �����
	wstring ClassName;//��� ������ ��� ����/���������
	wstring WindowName;
	Position position;
	int w, h, x, y;
	int borderRadius = 0;
	BackBuffer* bkBuffer=new BackBuffer;
	WindowHandler windowHandler;
	Painter* painter;
public:
	Frame* parent;//������������ ����
	DWORD WindowStyle;
	int wReal, hReal;//������� � ��������
	int xReal, yReal;//������� � ��������
	HWND hWnd = 0;
	EventHandler* eventHandler = new EventHandler();
	Font* font=new Font();
	Frame(HINSTANCE hInstance, wstring ClassName, wstring WindowName, DWORD style, Position pos, int x, int y, int w, int h,Frame* parent);
	virtual void CreateWnd();
	void SetBorder(Border border,BorderNum bn);
	void SetBackgroundColor(COLORREF cl);
	void CheckSize();//��������� ����� �������� ������� � �������� ��� ������� ��������
	HDC PaintWindow(HDC hdc,RECT& rect);//�������� ������� ��������� ��� ��������� ���� � ���������� ����� ��� ����������
	void EndPaintWindow(HDC hdc,RECT& rect,bool isTransparentBackground=0);//������� ���������� ������� ��� ���������
	void SetBorderRadius(int r);
	void SetPicture(HBITMAP bmp,bool isTransparent=0,COLORREF transparent_color=RGB(255,255,255));
	void SetOwnerDraw(DrawFunction df);
	void MoveWindow(int x, int y, int w, int h);
	void OffsetWindow(int dx, int dy);
	void SetPos(Position pos, int x, int y, int w, int h);
	void Repaint();
	void HideWindow();
	void ShowWindow();
	virtual ~Frame() { delete font; delete bkBuffer; delete eventHandler; delete painter; }
	friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//������� ������� ���������
};





//������, ������� ������ ���� ���������� - ��������� �� ������

extern map<HWND, Frame*> windowTree;

Frame* GetWindowByHWND(HWND);

void FinallizeFramework();