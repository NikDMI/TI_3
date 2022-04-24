#pragma once
#include "Frame.h"
#include "CWindow.h"
#include "Button.h"
#include "Label.h"
#include "Edit.h"
#include "Font.h"
#include "BackBuffer.h"
#include "EventHandler.h"
#include "WindowHandler.h"
#include "ScrollBar.h"
#include "ListBox.h"
#include "CaptionClass.h"
#include "Painter.h"
#include "Table.h"
#include "TextDrawing.h"

//#include <gdiplus.h>

//#pragma comment (lib,"Gdiplus.lib")

void InitWindowFramework();
void FinallizeFramework();
void StartWindowLoop();
void StopWindowLoop();
void FreeFontResourses();