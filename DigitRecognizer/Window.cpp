#include "Window.h"
#include "RendererDX11.h"
#include "Renderer.h"

LRESULT Window::OnUpdate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

void Window::InitRenderer()
{
	Window::renderer = std::static_pointer_cast<RendererBase>(std::make_shared<RendererDX11>());

	Renderer::InitRenderer(static_cast<RendererBase*>(Window::renderer.get()));
}

int Window::OnInit(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	HWND                hWnd;
	MSG                 msg;
	WNDCLASS            wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = Window::OnUpdate;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = NULL;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT("DigitRecognizer");

	RegisterClass(&wndClass);

	hWnd = CreateWindow(
		TEXT("DigitRecognizer"),   // window class name
		TEXT("DigitRecognizer"),  // window caption
		WS_OVERLAPPED | WS_SYSMENU,      // window style
		CW_USEDEFAULT,            // initial x position
		CW_USEDEFAULT,            // initial y position
		SCREEN_WIDTH,  // initial x size
		SCREEN_HEIGHT / 0.935f,  // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters

	ShowWindow(hWnd, iCmdShow);

	Window::InitRenderer();

	Renderer::OnInit(hWnd);
	UpdateWindow(hWnd);
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
		Renderer::OnUpdate();
	}
	Renderer::OnStop();

	return (int)msg.wParam;
}

std::shared_ptr<RendererBase> Window::renderer;