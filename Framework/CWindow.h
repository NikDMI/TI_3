#pragma once
#include "Frame.h"

namespace Window {
	extern wstring ClassName;
	enum class WindowType { MainWindow,PanelWindow };

	void InitWindowClass();
}

class WindowClass :public Frame {
protected:
	vector<Frame*> childControls;//дочерние элементы окна
public:
	Window::WindowType type;//тип окна
	WindowClass(wstring WindowName, Window::WindowType type, Position pos, int x, int y, int w, int h, Frame* parent);
	void CreateWnd();
	void AddChild(Frame* child);
	void ChangeChildrenSize();
};

