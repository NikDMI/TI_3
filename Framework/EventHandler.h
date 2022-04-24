#pragma once
#include <Windows.h>

class Frame;
typedef void (*BasicEvent)(Frame* btn);
enum class MouseEvents { OnClick, OnMove, OnLeave, OnLBDown };
enum class FocusEvents {OnFocus,OnKillFocus};

extern Frame* FocusedWindow;

//класс обработчка событий
class EventHandler {
protected:
	//Frame* frame;
	BasicEvent OnClick = NULL;
	BasicEvent OnMove = NULL;
	BasicEvent OnLeave = NULL;
	BasicEvent OnLBDown = NULL;
	BasicEvent OnFocus = NULL;
	BasicEvent OnKillFocus = NULL;
	int flagLeave = -1;
	void CreateLeave(HWND hWnd);
public:
	void SetMouseEvent(MouseEvents me, BasicEvent act);
	void SetFocusEvent(FocusEvents fe, BasicEvent act);
	void PlayMouseClick(Frame* frame);
	void PlayMouseLBDown(Frame* frame);
	void PlayMouseMove(Frame* frame, HWND hWnd);
	void PlayMouseLeave(Frame* frame);
	void PlayFocusEvent(Frame* frame);
	void PlayKillFocusEvent(Frame* frame);
	EventHandler();
};