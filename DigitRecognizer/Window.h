#pragma once
#include <windows.h>
class RendererBase;

class Window
{
	static std::shared_ptr<RendererBase> renderer;
	static LRESULT OnUpdate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void InitRenderer();
	Window();
public:
	static int OnInit(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow);
};