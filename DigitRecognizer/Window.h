#pragma once
#include <windows.h>

class Window
{
	static Window window;
	static LRESULT OnUpdate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	Window();
public:
	static int OnInit(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow);
};