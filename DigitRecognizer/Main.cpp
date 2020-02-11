/*
main с загрузкой датасета в память
*/

#include "Dataset.h"
#include "Window.h"


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int iCmdShow)
{
	Dataset data("train-labels.idx1-ubyte", "train-images.idx3-ubyte", "t10k-labels.idx1-ubyte", "t10k-images.idx3-ubyte");	

	return Window::OnInit(hInstance, hPrevInstance, lpCmdLine, iCmdShow);
}

