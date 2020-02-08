/*
main с загрузкой датасета в память
*/
#include <windows.h>

#include "SingleCoreDG.h"

int digit_recognizer()
{
	std::vector<DigitRecognizer_up> Recs;
	Recs.push_back(std::make_unique<SingleCoreDG>());

	std::for_each(Recs.begin(), Recs.end(), [](DigitRecognizer_up& R)
	{
		R->SetData("train-labels.idx1-ubyte", "train-images.idx3-ubyte", "t10k-labels.idx1-ubyte", "t10k-images.idx3-ubyte");
		R->PrintRandom();
	});
	return 0;
}


void OnPaint(HWND hWnd)
{
	PAINTSTRUCT  ps;
	HDC          hdc;
	InvalidateRect(hWnd, NULL, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	HWND                hWnd;
	MSG                 msg;
	WNDCLASS            wndClass;


	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = NULL;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT("DigitRecognizer");

	RegisterClass(&wndClass);

	hWnd = CreateWindow(
		TEXT("DigitRecognizer"),   // window class name
		TEXT("DigitRecognizer"),  // window caption
		WS_OVERLAPPED | WS_SYSMENU,      // window style
		CW_USEDEFAULT,            // initial x position
		CW_USEDEFAULT,            // initial y position
		800,  // initial x size
		600,  // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	switch (message)
	{
	case WM_PAINT:
		OnPaint(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
