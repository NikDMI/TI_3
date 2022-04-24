#pragma once
#include <Windows.h>
class Frame;

using MessageProc = void (*)(Frame* frame,WPARAM wParam,LPARAM lParam);

void NullMessageProc(Frame* frame, WPARAM wParam, LPARAM lParam);
//void NullMessageProcSize(Frame* frame, WPARAM wParam, LPARAM lParam);

struct WindowHandler {
	MessageProc OnSize = NullMessageProc;
	MessageProc OnPaint = NullMessageProc;
	MessageProc OnClose = NullMessageProc;
	MessageProc OnLBDown = NullMessageProc;
	MessageProc OnLBUp = NullMessageProc;
	MessageProc OnMouseMove = NullMessageProc;
	MessageProc OnMouseLeave = NullMessageProc;
	MessageProc OnSetFocus = NullMessageProc;
	MessageProc OnKillFocus = NullMessageProc;
	MessageProc OnKeyDown = NullMessageProc;
	MessageProc OnChar = NullMessageProc;
};