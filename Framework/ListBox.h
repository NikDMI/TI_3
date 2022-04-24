#pragma once

#include "Frame.h"
#include "Painter.h"


namespace ListBox {
	extern wstring ClassName;
	void InitWindowClass();
}



class ListBoxClass :public Frame {
protected:
	struct listItemInfo {
		wstring text;
		int height;//высота в базовых линиях
	};
	int atomicSize=5;//кол-во базовых линий в списке
	vector<listItemInfo> listItems;
	double itemsScroll = 0;//сколько базовых линий проскролено
	int dxText=0;//насколько текст отодвинуть вправо
	//вспомогательная инфа
	bool isMoved = false;//помещен ли курсор на лист
	bool isPushed = false;//нажата ли мышь
	bool isChoose = false;//отпустили ли клавишу
	int xMouse, yMouse;
	int listID = -1;//выбранный элемент

	void PaintListBox(HDC hdc, RECT r);
	friend void ListBoxClass_OnPaint(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void ListBoxClass_OnMove(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void ListBoxClass_OnLeave(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void ListBoxClass_OnLDown(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void ListBoxClass_OnLUp(Frame* frame, WPARAM wParam, LPARAM lParam);
public:
	ComplexPainter* complexPainter = new ComplexPainter();//основной стиль
	ListBoxClass(Position pos, int x, int y, int w, int h, Frame* parent);
	~ListBoxClass();
	void SetListInfo(int basicLineCount, double linesScroll);
	void SetListScroll(double scroll);
	void AddItem(wstring text, int height);
	void ChangeItemInfo(int index, wstring text, int height);
	void RemoveItem(int index);
	int GetItemID();
	void SetItemID(int index);
	void SetTextDX(int percentsDX);
	void ClearList();
};
